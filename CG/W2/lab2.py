import cv2 as cv
import numpy as np
from math import *

img = cv.imread('Lenna.png')

## 1 y 2. Color Scale
def changeColorScale(img, target_color):
  width, height = img.shape[:2]
  new_img = np.zeros((width, height, 3), dtype=np.float32)
  target_color = np.array(target_color, dtype=np.float32)

  for i in range(width):
    for j in range(height):
      new_img[i, j] = max(img[i, j]) / max(target_color) * target_color

  new_img = new_img.astype(np.uint8)
  return new_img

# new_img = changeColorScale(img, (100, 100, 100))
# cv.imshow("Display window", new_img)

## 3. Create color regios on a image or put an image (combinar)
def colorRegions(orig_img, bg_img):

  return

cv.imshow("Display window", new_img)
k = cv.waitKey(0)
cv.destroyAllWindows()

