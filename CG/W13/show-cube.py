import cv2
import numpy as np

def draw_3d_cube(frame, homography_matrix, marker_size):
    """
    Dibuja un cubo 3D sobre el marcador detectado usando las coordenadas del archivo PLY
    """
    # Definir los vértices del cubo con una escala más apropiada
    size = min(marker_size) * 0.3  # Hacer el cubo más pequeño
    height = size * 0.8  # Altura del cubo más proporcionada
    
    # Vértices del cubo en 3D (x, y, z) - coordenadas relativas al marcador
    cube_3d = np.float32([
        # Base inferior (z=0)
        [0, 0, 0],
        [size, 0, 0],
        [size, size, 0],
        [0, size, 0],
        # Base superior (z negativo para que se vea hacia arriba)
        [0, 0, -height],
        [size, 0, -height],
        [size, size, -height],
        [0, size, -height]
    ])
    
    # Centrar el cubo en el marcador
    offset_x = (marker_size[1] - size) / 2
    offset_y = (marker_size[0] - size) / 2
    cube_3d[:, 0] += offset_x
    cube_3d[:, 1] += offset_y
    
    # Parámetros de la cámara mejorados
    camera_matrix = np.array([
        [frame.shape[1], 0, frame.shape[1]/2],
        [0, frame.shape[1], frame.shape[0]/2],
        [0, 0, 1]
    ], dtype=np.float32)
    
    # Vector de distorsión (asumimos sin distorsión)
    dist_coeffs = np.zeros((4, 1))
    
    # Extraer rotación y traslación de la homografía
    # Puntos del marcador en 2D
    marker_corners_2d = np.array([
        [0, 0],
        [marker_size[1], 0],
        [marker_size[1], marker_size[0]],
        [0, marker_size[0]]
    ], dtype=np.float32)
    
    # Puntos del marcador en 3D (plano z=0)
    marker_corners_3d = np.array([
        [0, 0, 0],
        [marker_size[1], 0, 0],
        [marker_size[1], marker_size[0], 0],
        [0, marker_size[0], 0]
    ], dtype=np.float32)
    
    # Encontrar correspondencias en el frame actual usando la homografía
    marker_2d_transformed = cv2.perspectiveTransform(
        marker_corners_2d.reshape(-1, 1, 2), 
        homography_matrix
    ).reshape(-1, 2)
    
    # Resolver PnP para obtener pose 3D
    success, rvec, tvec = cv2.solvePnP(
        marker_corners_3d, 
        marker_2d_transformed, 
        camera_matrix, 
        dist_coeffs
    )
    
    if success:
        # Proyectar los vértices del cubo 3D al plano 2D
        cube_2d, _ = cv2.projectPoints(cube_3d, rvec, tvec, camera_matrix, dist_coeffs)
        cube_2d = np.int32(cube_2d).reshape(-1, 2)
        
        # Definir las caras del cubo correctamente
        faces = [
            [0, 1, 2, 3],  # Base inferior
            [4, 7, 6, 5],  # Base superior  
            [0, 4, 5, 1],  # Cara frontal
            [2, 6, 7, 3],  # Cara trasera
            [0, 3, 7, 4],  # Cara izquierda
            [1, 5, 6, 2]   # Cara derecha
        ]
        
        # Colores para cada cara (BGR) - más suaves
        colors = [
            (100, 100, 255),  # Rojo claro - base inferior
            (100, 255, 100),  # Verde claro - base superior
            (255, 100, 100),  # Azul claro - frontal
            (100, 255, 255),  # Amarillo claro - trasera
            (255, 100, 255),  # Magenta claro - izquierda
            (255, 255, 100)   # Cian claro - derecha
        ]
        
        # Dibujar las caras del cubo con mejor validación
        for i, face in enumerate(faces):
            pts = cube_2d[face]
            # Verificar que todos los puntos estén dentro del frame con margen
            margin = 50
            if all(-margin <= pt[0] <= frame.shape[1] + margin and 
                   -margin <= pt[1] <= frame.shape[0] + margin for pt in pts):
                # Asegurar que los puntos sean válidos
                valid_pts = []
                for pt in pts:
                    x = max(0, min(frame.shape[1] - 1, int(pt[0])))
                    y = max(0, min(frame.shape[0] - 1, int(pt[1])))
                    valid_pts.append([x, y])
                
                if len(valid_pts) == 4:
                    valid_pts = np.array(valid_pts, dtype=np.int32)
                    cv2.fillPoly(frame, [valid_pts], colors[i])
                    cv2.polylines(frame, [valid_pts], True, (0, 0, 0), 2)
        
        # Dibujar las aristas del cubo para mayor claridad
        edges = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # Base inferior
            (4, 5), (5, 6), (6, 7), (7, 4),  # Base superior
            (0, 4), (1, 5), (2, 6), (3, 7)   # Aristas verticales
        ]
        
        for edge in edges:
            pt1, pt2 = cube_2d[edge[0]], cube_2d[edge[1]]
            # Verificar que los puntos estén dentro del frame con margen
            margin = 50
            if (-margin <= pt1[0] <= frame.shape[1] + margin and 
                -margin <= pt1[1] <= frame.shape[0] + margin and
                -margin <= pt2[0] <= frame.shape[1] + margin and 
                -margin <= pt2[1] <= frame.shape[0] + margin):
                
                # Asegurar que los puntos sean válidos
                x1 = max(0, min(frame.shape[1] - 1, int(pt1[0])))
                y1 = max(0, min(frame.shape[0] - 1, int(pt1[1])))
                x2 = max(0, min(frame.shape[1] - 1, int(pt2[0])))
                y2 = max(0, min(frame.shape[0] - 1, int(pt2[1])))
                
                cv2.line(frame, (x1, y1), (x2, y2), (0, 0, 0), 2)

# Carga de imagen
marker_image = cv2.imread('marker.png', cv2.IMREAD_GRAYSCALE)
if marker_image is None:
    print("Error: No se pudo cargar 'marker.png'. Asegúrate de que el archivo existe.")
    exit()

orb = cv2.ORB_create()
kp_marker, des_marker = orb.detectAndCompute(marker_image, None) 

if des_marker is None:
    print("Error: No se pudieron detectar características en el marcador.")
    exit()

bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: No se pudo abrir la cámara.")
    exit()

print("Presiona 'q' para salir")

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: No se pudo leer el frame de la cámara.")
        break

    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    kp_frame, des_frame = orb.detectAndCompute(gray_frame, None)
    
    if des_frame is not None:
        matches = bf.match(des_marker, des_frame)
        matches = sorted(matches, key=lambda x: x.distance)

        if len(matches) > 100:  # Reducir el umbral para mejor detección
            try:
                src_pts = np.float32([kp_marker[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp_frame[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)
                matrix, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
                
                if matrix is not None:
                    h, w = marker_image.shape
                    pts = np.float32([[0, 0], [0, h], [w, h], [w, 0]]).reshape(-1, 1, 2)
                    dst = cv2.perspectiveTransform(pts, matrix)
                    
                    # Dibujar contorno del marcador detectado
                    cv2.polylines(frame, [np.int32(dst)], True, (0, 255, 0), 3)
                    
                    # Dibujar el cubo 3D
                    draw_3d_cube(frame, matrix, (h, w))
                    
            except cv2.error as e:
                print(f"Error en el procesamiento: {e}")
                continue

    cv2.imshow('AR Overlay with 3D Cube', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()