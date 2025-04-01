import cv2
import mediapipe as mp

# Inicializa MediaPipe Face Mesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(static_image_mode=False,
                                   max_num_faces=1,
                                   refine_landmarks=True,
                                   min_detection_confidence=0.5,
                                   min_tracking_confidence=0.5)

# Dibujo de los puntos
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

# Iniciar la cámara
cap = cv2.VideoCapture(0)  # Usa 1 o 2 si tienes más cámaras

while cap.isOpened():
    success, frame = cap.read()
    if not success:
        print("No se pudo acceder a la cámara.")
        break

    # Convertir a RGB
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Procesar la imagen
    results = face_mesh.process(frame_rgb)

    # Dibujar puntos si se detecta un rostro
    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            mp_drawing.draw_landmarks(
                image=frame,
                landmark_list=face_landmarks,
                connections=mp_face_mesh.FACEMESH_TESSELATION,
                landmark_drawing_spec=None,
                connection_drawing_spec=mp_drawing_styles
                    .get_default_face_mesh_tesselation_style())
            
            # Opcional: dibujar contornos de ojos, labios, etc.
            mp_drawing.draw_landmarks(
                image=frame,
                landmark_list=face_landmarks,
                connections=mp_face_mesh.FACEMESH_CONTOURS,
                landmark_drawing_spec=None,
                connection_drawing_spec=mp_drawing_styles
                    .get_default_face_mesh_contours_style())

    # Mostrar la ventana
    cv2.imshow('Face Landmarks - MediaPipe', frame)

    # Salir con ESC
    if cv2.waitKey(1) & 0xFF == 27:
        break

# Liberar
cap.release()
cv2.destroyAllWindows()