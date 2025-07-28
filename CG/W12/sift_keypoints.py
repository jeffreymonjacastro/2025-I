import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# Cargar la imagen en escala de grises
img = cv.imread('./img/utec1.jpg', cv.IMREAD_GRAYSCALE) 

# Verificar si la imagen se cargó correctamente
if img is None:
    print("Error: No se pudo cargar la imagen")
    exit()

# Crear el detector SIFT
sift = cv.SIFT_create()

# Detectar keypoints y descriptores con SIFT
keypoints, descriptors = sift.detectAndCompute(img, None)

print(f"Se encontraron {len(keypoints)} keypoints")

# Convertir la imagen a color para poder dibujar en rojo
img_color = cv.cvtColor(img, cv.COLOR_GRAY2BGR)

# Dibujar los keypoints como puntos rojos
img_with_keypoints = cv.drawKeypoints(img_color, keypoints, None, 
                                     color=(0, 0, 255),  # Color rojo en formato BGR
                                     flags=cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# Guardar la imagen con los keypoints
cv.imwrite('sift_keypoints.jpg', img_with_keypoints)

# Mostrar la imagen (opcional)
# plt.figure(figsize=(10, 8))
# plt.imshow(cv.cvtColor(img_with_keypoints, cv.COLOR_BGR2RGB))
# plt.title(f'SIFT Keypoints detectados: {len(keypoints)}')
# plt.axis('off')
# plt.show()

print("Imagen guardada como 'sift_keypoints.jpg'")