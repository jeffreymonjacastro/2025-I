import cv2 
import numpy as np
import time
import platform

last_update_time = time.time()

def contrast(
  factor: float = 1.0) -> np.ndarray:

  img = cv2.imread("../lowcontrast.png")
  img = img.astype(np.float32)

  old_m = np.min(img)
  old_M = np.max(img)
  middle = 128

  new_m = middle - factor * middle
  new_M = middle + factor * (255 - middle)

  img = new_m + ((img - old_m) / (old_M - old_m)) * (new_M - new_m)
  img = np.clip(img, 0, 255).astype(np.uint8)

  return img


def update_contrast(val):
  global last_update_time
  val = val / 100.0

  current_time = time.time()
  if platform.system() == 'Windows' and (current_time - last_update_time) < 0.05: 
    return

  last_update_time = current_time

  result = contrast(val)
  cv2.imshow("Contrast", result)


cv2.namedWindow("Contrast", cv2.WINDOW_NORMAL)
cv2.createTrackbar("(%)", "Contrast", 100, 200, update_contrast)

is_windows = platform.system() == 'Windows'

if is_windows:
  original_img = cv2.imread("../lowcontrast.png")
  height, width = original_img.shape[:2]
  cv2.resizeWindow("Contrast", width, height + 50)

update_contrast(100)

# while True:
#   key = cv2.waitKey(1) & 0xFF
#   if key == 27:  
#     break
cv2.waitKey(0)
cv2.destroyAllWindows()