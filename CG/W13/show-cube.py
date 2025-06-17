import cv2
import numpy as np

def draw_3d_cube(frame, homography_matrix, marker_size):
    """
    Dibuja un cubo 3D sobre el marcador detectado usando las coordenadas del archivo PLY
    """
    # Definir los vértices del cubo basados en el archivo PLY (escalados al tamaño del marcador)
    scale = marker_size[0] / 4  # Escalar el cubo al tamaño del marcador
    height = scale * 1.5  # Altura del cubo
    
    # Vértices del cubo en 3D (x, y, z)
    cube_3d = np.float32([
        [-1, -1, 0],    # Base inferior
        [1, -1, 0],     
        [1, 1, 0],      
        [-1, 1, 0],     
        [-1, -1, -height], # Base superior (z negativo para que se vea hacia arriba)
        [1, -1, -height],  
        [1, 1, -height],   
        [-1, 1, -height]   
    ]) * scale
    
    # Mover el cubo al centro del marcador
    cube_3d[:, 0] += marker_size[1] / 2  # Centrar en x
    cube_3d[:, 1] += marker_size[0] / 2  # Centrar en y
    
    # Parámetros de la cámara (aproximados - en una aplicación real deberías calibrar)
    camera_matrix = np.array([
        [800, 0, frame.shape[1]/2],
        [0, 800, frame.shape[0]/2],
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
        
        # Definir las caras del cubo basadas en el archivo PLY
        faces = [
            [3, 2, 1, 0],  # Base inferior
            [0, 1, 5, 4],  # Cara frontal
            [2, 3, 7, 6],  # Cara trasera
            [4, 7, 3, 0],  # Cara izquierda
            [1, 2, 6, 5],   # Cara derecha
            [4, 5, 6, 7],  # Base superior  
        ]
        
        # Colores para cada cara (BGR)
        colors = [
            (0, 0, 255),     # Rojo - base inferior
            (0, 255, 0),     # Verde - base superior
            (255, 0, 0),     # Azul - frontal
            (0, 255, 255),   # Amarillo - trasera
            (255, 0, 255),   # Magenta - izquierda
            (255, 255, 0)    # Cian - derecha
        ]
        
        # Dibujar las caras del cubo
        for i, face in enumerate(faces):
            pts = cube_2d[face]
            # Verificar que todos los puntos estén dentro del frame
            if all(0 <= pt[0] < frame.shape[1] and 0 <= pt[1] < frame.shape[0] for pt in pts):
                cv2.fillPoly(frame, [pts], colors[i])
                cv2.polylines(frame, [pts], True, (0, 0, 0), 2)  # Contorno negro
        
        # Dibujar las aristas del cubo para mayor claridad
        edges = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # Base inferior
            (4, 5), (5, 6), (6, 7), (7, 4),  # Base superior
            (0, 4), (1, 5), (2, 6), (3, 7)   # Aristas verticales
        ]
        
        for edge in edges:
            pt1, pt2 = cube_2d[edge[0]], cube_2d[edge[1]]
            # Verificar que los puntos estén dentro del frame
            if (0 <= pt1[0] < frame.shape[1] and 0 <= pt1[1] < frame.shape[0] and
                0 <= pt2[0] < frame.shape[1] and 0 <= pt2[1] < frame.shape[0]):
                cv2.line(frame, tuple(pt1), tuple(pt2), (0, 0, 0), 2)

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

        if len(matches) > 125:
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