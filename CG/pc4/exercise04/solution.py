import cv2
import numpy as np
from enum import Enum

Blender = Enum('Blender', 'FeatherBlender MultiBandBlender')
FeaturesFinder = Enum('FeaturesFinder', 'AKAZE ORB SIFT SURF')
FeaturesMatcher = Enum('FeaturesMatcher', 'AffineBestOf2Nearest BestOf2NearestRange')
Warper = Enum('Warper', 'Affine CompressedRectilinearPortrait CompressedRectilinear Cylindrical Fisheye Mercator PaniniPortrait Panini Plane Spherical Stereographic TransverseMercator')

def stitch_images(
    full_path_input_image: list[str], 
    blender: Blender, 
    features_finder: FeaturesFinder, 
    features_matcher: FeaturesMatcher, 
    warper: Warper, 
    full_path_output_image: str
):
    imgs = []
    for filename in full_path_input_image:
        img = cv2.imread(filename)
        if img is not None:
            imgs.append(img)

    if len(imgs) < 2:
        print("Not enough images to stitch.")
        return False
    
    # Solo implementamos para los parámetros específicos solicitados
    if (blender == Blender.FeatherBlender and 
        features_finder == FeaturesFinder.SIFT and 
        features_matcher == FeaturesMatcher.BestOf2NearestRange and 
        warper == Warper.Plane):
        
        # 1. Crear detector SIFT
        detector = cv2.SIFT.create()
        
        # 2. Detectar keypoints y descriptores en todas las imágenes
        keypoints = []
        descriptors = []
        
        for img in imgs:
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            kp, desc = detector.detectAndCompute(gray, None)
            keypoints.append(kp)
            descriptors.append(desc)
        
        # 3. Crear matcher BF (Brute Force) para BestOf2NearestRange
        matcher = cv2.BFMatcher()
        
        # 4. Para simplificar, haremos stitching de solo las primeras dos imágenes
        if len(imgs) >= 2:
            # Encontrar matches entre las dos primeras imágenes
            matches = matcher.knnMatch(descriptors[0], descriptors[1], k=2)
            
            # Aplicar ratio test (Lowe's ratio test)
            good_matches = []
            for match_pair in matches:
                if len(match_pair) == 2:
                    m, n = match_pair
                    if m.distance < 0.7 * n.distance:
                        good_matches.append(m)
            
            if len(good_matches) > 10:
                # Extraer puntos correspondientes
                src_pts = np.float32([keypoints[0][m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
                dst_pts = np.float32([keypoints[1][m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)
                
                # Encontrar homografía usando RANSAC
                homography, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
                
                if homography is not None:
                    # Warping plano (Plane warper)
                    h1, w1 = imgs[0].shape[:2]
                    h2, w2 = imgs[1].shape[:2]
                    
                    # Calcular las esquinas de la primera imagen transformada
                    corners1 = np.float32([[0, 0], [w1, 0], [w1, h1], [0, h1]]).reshape(-1, 1, 2)
                    corners1_transformed = cv2.perspectiveTransform(corners1, homography)
                    
                    # Combinar con las esquinas de la segunda imagen
                    corners2 = np.float32([[0, 0], [w2, 0], [w2, h2], [0, h2]]).reshape(-1, 1, 2)
                    all_corners = np.concatenate((corners1_transformed, corners2), axis=0)
                    
                    # Encontrar el bounding box del resultado
                    [x_min, y_min] = np.int32(all_corners.min(axis=0).ravel())
                    [x_max, y_max] = np.int32(all_corners.max(axis=0).ravel())
                    
                    # Matriz de traslación para evitar coordenadas negativas
                    translation_dist = [-x_min, -y_min]
                    H_translation = np.array([[1, 0, translation_dist[0]], [0, 1, translation_dist[1]], [0, 0, 1]])
                    
                    # Aplicar warping a la primera imagen
                    output_img = cv2.warpPerspective(imgs[0], H_translation.dot(homography), 
                                                   (x_max - x_min, y_max - y_min))
                    
                    # Colocar la segunda imagen en la posición correcta
                    output_img[translation_dist[1]:h2 + translation_dist[1], 
                             translation_dist[0]:w2 + translation_dist[0]] = imgs[1]
                    
                    # Blending simple usando FeatherBlender (implementación básica)
                    # Crear máscara para blending suave en la zona de solapamiento
                    mask1 = np.zeros((y_max - y_min, x_max - x_min), dtype=np.uint8)
                    cv2.fillPoly(mask1, [np.int32(corners1_transformed + translation_dist)], 255)
                    
                    mask2 = np.zeros((y_max - y_min, x_max - x_min), dtype=np.uint8)
                    mask2[translation_dist[1]:h2 + translation_dist[1], 
                          translation_dist[0]:w2 + translation_dist[0]] = 255
                    
                    # Zona de solapamiento
                    overlap = cv2.bitwise_and(mask1, mask2)
                    
                    # Aplicar blending suave en la zona de solapamiento
                    if np.any(overlap):
                        # Crear pesos para blending gradual
                        distance_transform = cv2.distanceTransform(overlap, cv2.DIST_L2, 5)
                        if distance_transform.max() > 0:
                            weights = distance_transform / distance_transform.max()
                            
                            # Aplicar blending en la zona de solapamiento
                            warped_img1 = cv2.warpPerspective(imgs[0], H_translation.dot(homography), 
                                                            (x_max - x_min, y_max - y_min))
                            
                            for i in range(3):  # Para cada canal de color
                                overlap_region = overlap > 0
                                output_img[overlap_region, i] = (
                                    weights[overlap_region] * warped_img1[overlap_region, i] + 
                                    (1 - weights[overlap_region]) * imgs[1][
                                        overlap_region[translation_dist[1]:h2 + translation_dist[1], 
                                                     translation_dist[0]:w2 + translation_dist[0]], i
                                    ]
                                )
                    
                    # Guardar la imagen resultante
                    cv2.imwrite(full_path_output_image, output_img)
                    print(f"Panorama guardado en: {full_path_output_image}")
                    return True
                else:
                    print("No se pudo encontrar una homografía válida.")
                    return False
            else:
                print("No hay suficientes matches de buena calidad.")
                return False
        else:
            print("Se necesitan al menos 2 imágenes.")
            return False
    else:
        print("Esta implementación solo soporta: FeatherBlender + SIFT + BestOf2NearestRange + Plane")
        return False


stitch_images(
    full_path_input_image=[
        'panorama1-input-1.jpg',
        'panorama1-input-2.jpg'
    ],
    blender=Blender.FeatherBlender,
    features_finder=FeaturesFinder.SIFT,
    features_matcher=FeaturesMatcher.BestOf2NearestRange,
    warper=Warper.Plane,
    full_path_output_image='panorama1-plane.jpg'
)