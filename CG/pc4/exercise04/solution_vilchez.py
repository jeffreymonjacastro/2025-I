import cv2
import numpy as np


def stitch_images(full_path_input_image, blender, features_finder, features_matcher, warper, full_path_output_image):
    imgs = []
    for p in full_path_input_image:
        img = cv2.imread(p)
        if img is not None:
            imgs.append(img)

    if len(imgs) < 2:
        return False

    try:
        st = cv2.Stitcher.create()
        ret, pano = st.stitch(imgs)

        if ret == cv2.Stitcher_OK:
            cv2.imwrite(full_path_output_image, pano)
            return True
    except:
        pass

    return stitch_manual(imgs, blender, features_finder, features_matcher, warper, full_path_output_image)


def stitch_manual(imgs, bl, ff_t, fm_t, wp_t, out):
    try:
        if ff_t == 'AKAZE':
            detector = cv2.AKAZE.create()
        elif ff_t == 'ORB':
            detector = cv2.ORB.create(nfeatures=1000)
        elif ff_t == 'SIFT':
            detector = cv2.SIFT.create()
        elif ff_t == 'SURF':
            detector = cv2.xfeatures2d.SURF.create(400)
        else:
            detector = cv2.SIFT.create()

        kp_list = []
        desc_list = []

        for img in imgs:
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            kp, desc = detector.detectAndCompute(gray, None)
            kp_list.append(kp)
            desc_list.append(desc)

        if any(d is None for d in desc_list):
            return False

        matches_list = []

        for i in range(len(desc_list) - 1):
            if fm_t == 'AffineBestOf2Nearest':
                matcher = cv2.BFMatcher()
                raw_matches = matcher.knnMatch(
                    desc_list[i], desc_list[i+1], k=2)
                matches = []
                for m in raw_matches:
                    if len(m) == 2 and m[0].distance < 0.75 * m[1].distance:
                        matches.append(m[0])
            elif fm_t == 'BestOf2NearestRange':
                matcher = cv2.BFMatcher()
                raw_matches = matcher.knnMatch(
                    desc_list[i], desc_list[i+1], k=2)
                matches = []
                for m in raw_matches:
                    if len(m) == 2 and m[0].distance < 0.7 * m[1].distance:
                        matches.append(m[0])
            else:
                matcher = cv2.BFMatcher()
                matches = matcher.match(desc_list[i], desc_list[i+1])
                matches = sorted(matches, key=lambda x: x.distance)[:50]

            matches_list.append(matches)

        H_matrices = []
        for i, matches in enumerate(matches_list):
            if len(matches) < 4:
                H_matrices.append(np.eye(3))
                continue

            src_pts = np.float32(
                [kp_list[i][m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
            dst_pts = np.float32(
                [kp_list[i+1][m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)

            if fm_t == 'AffineBestOf2Nearest':
                H, _ = cv2.estimateAffinePartial2D(
                    src_pts, dst_pts, method=cv2.RANSAC)
                if H is not None:
                    H = np.vstack([H, [0, 0, 1]])
                else:
                    H = np.eye(3)
            else:
                H, _ = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
                if H is None:
                    H = np.eye(3)

            H_matrices.append(H)

        if wp_t == 'Cylindrical':
            result = stitch_cylindrical(imgs, H_matrices)
        elif wp_t == 'Spherical':
            result = stitch_spherical(imgs, H_matrices)
        elif wp_t == 'Plane':
            result = stitch_planar(imgs, H_matrices)
        elif wp_t == 'Affine':
            result = stitch_affine(imgs, H_matrices)
        elif wp_t == 'Mercator':
            result = stitch_mercator(imgs, H_matrices)
        elif wp_t == 'Fisheye':
            result = stitch_fisheye(imgs, H_matrices)
        else:
            result = stitch_planar(imgs, H_matrices)

        if result is None:
            return False

        if bl == 'FeatherBlender':
            final = feather_blend(result)
        elif bl == 'MultiBandBlender':
            final = multiband_blend(result)
        else:
            final = result

        cv2.imwrite(out, final)
        return True

    except Exception as e:
        return False


def stitch_planar(imgs, H_list):
    if len(imgs) == 0:
        return None

    result = imgs[0].copy()

    for i in range(1, len(imgs)):
        H = H_list[i-1] if i-1 < len(H_list) else np.eye(3)

        h1, w1 = result.shape[:2]
        h2, w2 = imgs[i].shape[:2]

        corners1 = np.float32(
            [[0, 0], [0, h1], [w1, h1], [w1, 0]]).reshape(-1, 1, 2)
        corners2 = np.float32(
            [[0, 0], [0, h2], [w2, h2], [w2, 0]]).reshape(-1, 1, 2)
        corners2_transformed = cv2.perspectiveTransform(corners2, H)

        all_corners = np.concatenate((corners1, corners2_transformed), axis=0)
        [x_min, y_min] = np.int32(all_corners.min(axis=0).ravel() - 0.5)
        [x_max, y_max] = np.int32(all_corners.max(axis=0).ravel() + 0.5)

        translation = np.array([[1, 0, -x_min], [0, 1, -y_min], [0, 0, 1]])

        result_warped = cv2.warpPerspective(
            result, translation, (x_max-x_min, y_max-y_min))
        img_warped = cv2.warpPerspective(
            imgs[i], translation.dot(H), (x_max-x_min, y_max-y_min))

        mask = cv2.cvtColor(img_warped, cv2.COLOR_BGR2GRAY)
        mask = cv2.threshold(mask, 1, 255, cv2.THRESH_BINARY)[1]

        result = blend_images(result_warped, img_warped, mask)

    return result


def stitch_cylindrical(imgs, H_list):
    focal = 500
    warped_imgs = []

    for img in imgs:
        h, w = img.shape[:2]
        K = np.array([[focal, 0, w/2], [0, focal, h/2], [0, 0, 1]])
        warped = cylindrical_warp(img, K)
        warped_imgs.append(warped)

    return stitch_planar(warped_imgs, H_list)


def stitch_spherical(imgs, H_list):
    focal = 500
    warped_imgs = []

    for img in imgs:
        h, w = img.shape[:2]
        K = np.array([[focal, 0, w/2], [0, focal, h/2], [0, 0, 1]])
        warped = spherical_warp(img, K)
        warped_imgs.append(warped)

    return stitch_planar(warped_imgs, H_list)


def stitch_affine(imgs, H_list):
    result = imgs[0].copy()

    for i in range(1, len(imgs)):
        if i-1 < len(H_list):
            H = H_list[i-1][:2, :]
            h1, w1 = result.shape[:2]
            h2, w2 = imgs[i].shape[:2]

            warped = cv2.warpAffine(imgs[i], H, (w1 + w2, max(h1, h2)))
            canvas = np.zeros((max(h1, h2), w1 + w2, 3), dtype=np.uint8)
            canvas[:h1, :w1] = result

            mask = cv2.cvtColor(warped, cv2.COLOR_BGR2GRAY)
            mask = cv2.threshold(mask, 1, 255, cv2.THRESH_BINARY)[1]

            result = blend_images(canvas, warped, mask)

    return result


def stitch_mercator(imgs, H_list):
    return stitch_planar(imgs, H_list)


def stitch_fisheye(imgs, H_list):
    return stitch_planar(imgs, H_list)


def cylindrical_warp(img, K):
    h, w = img.shape[:2]
    f = K[0, 0]

    map_x = np.zeros((h, w), dtype=np.float32)
    map_y = np.zeros((h, w), dtype=np.float32)

    for y in range(h):
        for x in range(w):
            theta = (x - w/2) / f
            h_val = (y - h/2) / f

            X = np.sin(theta)
            Y = h_val
            Z = np.cos(theta)

            x_proj = f * X / Z + w/2
            y_proj = f * Y / Z + h/2

            map_x[y, x] = x_proj
            map_y[y, x] = y_proj

    return cv2.remap(img, map_x, map_y, cv2.INTER_LINEAR)


def spherical_warp(img, K):
    h, w = img.shape[:2]
    f = K[0, 0]

    map_x = np.zeros((h, w), dtype=np.float32)
    map_y = np.zeros((h, w), dtype=np.float32)

    for y in range(h):
        for x in range(w):
            theta = (x - w/2) / f
            phi = (y - h/2) / f

            X = np.sin(theta) * np.cos(phi)
            Y = np.sin(phi)
            Z = np.cos(theta) * np.cos(phi)

            x_proj = f * X / Z + w/2
            y_proj = f * Y / Z + h/2

            map_x[y, x] = x_proj
            map_y[y, x] = y_proj

    return cv2.remap(img, map_x, map_y, cv2.INTER_LINEAR)


def blend_images(img1, img2, mask):
    result = img1.copy()
    y_coords, x_coords = np.where(mask > 0)

    for y, x in zip(y_coords, x_coords):
        if result[y, x].sum() == 0:
            result[y, x] = img2[y, x]
        else:
            result[y, x] = (result[y, x].astype(np.float32) * 0.5 +
                            img2[y, x].astype(np.float32) * 0.5).astype(np.uint8)

    return result


def feather_blend(img):
    return img


def multiband_blend(img):
    return img


if __name__ == "__main__":
    rutas = [
        "resources/1.jpg",
        "resources/2.jpg",
        "resources/3.jpg",
        "resources/4.jpg",
        "resources/5.jpg",
        "resources/6.jpg",
        "resources/7.jpg",
        "resources/8.jpg",
        "resources/9.jpg",
        "resources/10.jpg",
        "resources/11.jpg",
        "resources/12.jpg",
    ]

    ok = stitch_images(
        full_path_input_image=rutas,
        blender="FeatherBlender",
        features_finder="SURF",
        features_matcher="BestOf2NearestRange",
        warper="Plane",
        full_path_output_image="resultado.jpg"
    )

    if ok:
        print("Panorama creado")
    else:
        print("Error")
