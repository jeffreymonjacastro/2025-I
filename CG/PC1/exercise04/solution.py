import cv2 
import numpy as np

def change_color_scale():
  img = cv2.imread("./lenna.png", cv2.IMREAD_UNCHANGED)
  width, height = img.shape[:2]

  canvas = np.ones((width, height, 3), dtype=np.float32)
  cv2.circle(canvas, (width // 2, height // 2), width // 2, (255, 0, 0), -1)

  new_img = np.zeros((width, height, 3), dtype=np.float32)

  for i in range(width):
    for j in range(height):
      new_img[i, j] = max(img[i, j]) / max(canvas[i, j]) * canvas[i, j]

  new_img = new_img.astype(np.uint8)

  cv2.imwrite("./exercise04/output/lenna-colorscale.png", new_img)


change_color_scale()

