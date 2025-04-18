import cv2 
import numpy as np

def resize(
  InputImage: np.ndarray, 
  NEW_WIDTH: int, 
  NEW_HEIGHT: int, 
  PADDING_STRATEGY: str) -> np.ndarray:

  img = InputImage
  old_height, old_width = img.shape[:2]

  padding = 1

  NEW_HEIGHT = NEW_HEIGHT + 2 * padding
  NEW_WIDTH = NEW_WIDTH + 2 * padding

  scale_height = (old_height - 1) / (NEW_HEIGHT - 1)
  scale_width = (old_width - 1) / (NEW_WIDTH - 1)
  
  if PADDING_STRATEGY == "ZEROS":
    img = np.pad(img, pad_width=((padding, padding), (padding, padding)) + ((0, 0),) * (img.ndim - 2), mode='constant')
  elif PADDING_STRATEGY == "LAST_PIXEL":
    img = np.pad(img, pad_width=((padding, padding), (padding, padding)) + ((0, 0),) * (img.ndim - 2), mode='edge')
  else:
    raise ValueError("Invalid padding strategy. Use 'ZEROS' or 'LAST_PIXEL'.")

  output_image = np.zeros((NEW_HEIGHT, NEW_WIDTH) + img.shape[2:], dtype=img.dtype)

  for i in range(NEW_HEIGHT):
    for j in range(NEW_WIDTH):
      x = i * scale_height
      y = j * scale_width

      x0, x1 = int(np.floor(x)), int(np.ceil(x))
      if x1 >= old_height: x1 = old_height - 1

      y0, y1 = int(np.floor(y)), int(np.ceil(y))
      if y1 >= old_width: y1 = old_width - 1
      
      # 4 puntos alrededor del punto (x, y)
      f00 = img[x0, y0].astype(np.float32)
      f01 = img[x0, y1].astype(np.float32)
      f10 = img[x1, y0].astype(np.float32)
      f11 = img[x1, y1].astype(np.float32)

      if x1 != x0:
        f0 = f00 + (x - x0) / (x1 - x0) * (f10 - f00)
        f1 = f01 + (x - x0) / (x1 - x0) * (f11 - f01)
      else:
        f0 = f00
        f1 = f01

      if y1 != y0:
        f = f0 + (y - y0) / (y1 - y0) * (f1 - f0)
      else:
        f = f0

      output_image[i, j] = np.clip(f, 0, 255).astype(img.dtype) 
  
  output_image = output_image[padding:-padding, padding:-padding]

  return output_image


# img = cv2.imread("../lenna.png", cv2.IMREAD_UNCHANGED)
# result = resize(img, 900, 400, "LAST_PIXEL")
# print(result.shape)
# cv2.imshow("Display window", result)
# k = cv2.waitKey(0)
# cv2.destroyAllWindows()