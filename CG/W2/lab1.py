import cv2 as cv
import numpy as np
from math import *

img = cv.imread('lowcontrast.png')

## 1
## Brigthness


## 2 
## Contrast
def contrast(img, new_m, new_M):
  old_m = img.min()
  old_M = img.max()

  img = (img - old_m) * ((new_M - new_m) / (old_M - old_m)) + new_m

  img = np.clip(img, 0, 255).astype(np.uint8)
  
  return img


new_img = contrast(img, 0, 255)
cv.imshow("Display window", new_img)
k = cv.waitKey(0)
cv.destroyAllWindows()

## 3
## Tone Mapping HDR images
