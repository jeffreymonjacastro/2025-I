import cv2
import numpy as np

def parse_ply_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    header_end_index = -1
    for i, line in enumerate(lines):
        if 'end_header' in line:
            header_end_index = i
            break
    
    if header_end_index == -1:
        raise ValueError("No se encontró 'end_header' en el archivo PLY.")

    header = lines[:header_end_index+1]
    data = lines[header_end_index+1:]
    
    num_vertices = 0
    num_faces = 0
    has_colors = False

    for line in header:
        if line.startswith('element vertex'):
            num_vertices = int(line.split()[-1])
        elif line.startswith('element face'):
            num_faces = int(line.split()[-1])
        elif 'property uchar red' in line:
            has_colors = True
            
    vertices = []
    colors = []
    for i in range(num_vertices):
        parts = data[i].split()
        vertices.append([float(parts[0]), float(parts[1]), float(parts[2])])
        if has_colors and len(parts) >= 6:
            colors.append([int(parts[3]), int(parts[4]), int(parts[5])])
        else:
            colors.append([255, 255, 255])

    faces = []
    face_data_start_index = num_vertices
    for i in range(num_faces):
        parts = data[face_data_start_index + i].split()
        num_verts_in_face = int(parts[0])
        if num_verts_in_face == 3:
            faces.append([int(parts[1]), int(parts[2]), int(parts[3])])
        elif num_verts_in_face == 4:
            v0, v1, v2, v3 = int(parts[1]), int(parts[2]), int(parts[3]), int(parts[4])
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])

    return np.array(vertices), np.array(faces), np.array(colors), has_colors

def detect_qr_marker(image):
    qr_detector = cv2.QRCodeDetector()
    data, points, _ = qr_detector.detectAndDecode(image)
    if points is not None:
        return points.reshape(4, 2).astype(np.float32)
    return None

def calculate_pose(corners, image_shape, marker_size=1.0):
    height, width = image_shape[:2]
    focal_length = width
    camera_matrix = np.array([
        [focal_length, 0, width / 2],
        [0, focal_length, height / 2],
        [0, 0, 1]
    ], dtype=np.float32)
    
    s = marker_size / 2.0
    object_points = np.array([
        [-s,  s, 0],
        [ s,  s, 0],
        [ s, -s, 0],
        [-s, -s, 0]
    ], dtype=np.float32)

    dist_coeffs = np.zeros((4, 1))
    success, rvec, tvec = cv2.solvePnP(object_points, corners, camera_matrix, dist_coeffs)
    
    if success:
        return rvec, tvec, camera_matrix
    return None, None, None

def calculate_lighting(face_normal, light_direction):
    norm_face_normal = np.linalg.norm(face_normal)
    if norm_face_normal == 0:
        return 0
        
    face_normal = face_normal / norm_face_normal
    light_direction = light_direction / np.linalg.norm(light_direction)
    intensity = np.dot(face_normal, light_direction)
    return max(0, intensity)

def draw_mesh_on_top_of_marker(full_path_input_image, full_path_mesh, full_path_output_image):
    image = cv2.imread(full_path_input_image)
    if image is None:
        raise ValueError(f"No se pudo cargar la imagen: {full_path_input_image}")
    
    output_image = image.copy()
    
    corners = detect_qr_marker(image)
    if corners is None:
        print("No se detectó ningún marcador QR.")
        cv2.imwrite(full_path_output_image, output_image)
        return

    rvec, tvec, camera_matrix = calculate_pose(corners, image.shape, marker_size=1.0)
    
    if rvec is None:
        print("No se pudo calcular la pose del marcador.")
        cv2.imwrite(full_path_output_image, output_image)
        return

    vertices, faces, colors, has_colors = parse_ply_file(full_path_mesh)

    min_coords = np.min(vertices, axis=0)
    max_coords = np.max(vertices, axis=0)
    mesh_size = max_coords - min_coords
    
    max_dim = max(mesh_size[0], mesh_size[1], mesh_size[2])
    if max_dim == 0: max_dim = 1.0
    
    scale_factor = (1.0 / max(mesh_size[0], mesh_size[1])) * 0.8

    transformed_vertices = (vertices - (min_coords + max_coords) / 2) * scale_factor
    
    min_z_after_scaling = np.min(transformed_vertices, axis=0)[2]
    transformed_vertices[:, 2] -= min_z_after_scaling

    projected_vertices, _ = cv2.projectPoints(transformed_vertices, rvec, tvec, camera_matrix, np.zeros((4,1)))
    projected_vertices = np.squeeze(projected_vertices).astype(int)

    light_direction = np.array([0.5, -0.5, -1.0])
    
    R, _ = cv2.Rodrigues(rvec)
    cam_space_vertices = (R @ transformed_vertices.T).T + tvec.T
    face_depths = [np.mean(cam_space_vertices[face, 2]) for face in faces]
    sorted_face_indices = np.argsort(face_depths)[::-1]

    ambient_light = 0.3

    for face_idx in sorted_face_indices:
        face = faces[face_idx]
        
        v1, v2, v3 = transformed_vertices[face[0]], transformed_vertices[face[1]], transformed_vertices[face[2]]
        
        normal = np.cross(v2 - v1, v3 - v1)

        mesh_center = np.mean(transformed_vertices, axis=0)
        if np.dot(normal, v1 - mesh_center) < 0:
            normal = -normal

        rotated_normal = R @ normal
        intensity = calculate_lighting(rotated_normal, light_direction)
        total_intensity = ambient_light + (1.0 - ambient_light) * intensity
        
        pts = projected_vertices[face]
        
        if has_colors:
            base_color = np.mean([colors[i] for i in face], axis=0) / 255.0
            shaded_color = base_color * total_intensity
            shaded_bgr_255 = shaded_color[::-1] * 255
            bgr_color = (int(shaded_bgr_255[0]), int(shaded_bgr_255[1]), int(shaded_bgr_255[2]))
        else:
            gray_value = int(255 * total_intensity)
            bgr_color = (gray_value, gray_value, gray_value)
        
        cv2.fillPoly(output_image, [pts], bgr_color)
        cv2.polylines(output_image, [pts], isClosed=True, color=(20,20,20), thickness=1)

    cv2.imwrite(full_path_output_image, output_image)
    print(f"Imagen de realidad aumentada guardada en: {full_path_output_image}")

if __name__ == "__main__":
    draw_mesh_on_top_of_marker(
        full_path_input_image='markers/marker1.jpg',
        full_path_mesh='mesh/cube.ply',
        full_path_output_image='output.jpg'
    )