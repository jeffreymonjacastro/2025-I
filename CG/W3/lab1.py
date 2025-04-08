import cv2 as cv
import numpy as np
from math import *

def box(n):
  return np.ones((n, n), np.float32) / (n * n)

def bartlett(n):
  if n % 2 == 0:
    raise ValueError("n must be odd")

  l = [x for x in range(1, (n+1)//2)]
  l += [x for x in range((n+1)//2, 0, -1)]  

  m = [[l[i] * l[j] for j in range(n)] for i in range(n)]
  sum = np.sum(m) 

  m = np.array(m) / sum

  return m

def convolution(img, kernel, padding):
  width, height = img.shape[:2]

  for i in range(padding, width-padding):
    for j in range(padding, height-padding):
      for c in range(3):  
        region = img[i-padding:i+padding+1, j-padding:j+padding+1, c]
        filtered_value = np.sum(region * kernel)
        img[i, j, c] = filtered_value

  return img

def filter(image, type_kernel, size, border):
  img = cv.imread(image)
  width, height = img.shape[:2]
  padding = size // 2

  # Paddings
  if border == 'constant':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='constant')
  elif border == 'edge':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='edge')
  elif border == 'reflex':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='reflect')
  
  # Kernels
  if type_kernel == 'box':
    kernel = box(size) 
    img = convolution(img, kernel, padding)

  elif type_kernel == 'bartlett':
    kernel = bartlett(size) 
    img = convolution(img, kernel, padding)

  elif type_kernel == 'gaussian':
    kernel = cv.getGaussianKernel(size, 0) * cv.getGaussianKernel(size, 0).T
  elif type_kernel == 'laplacian':
    # kernel = np.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]], np.float32)
    pass

  img = img[padding:width-padding, padding:height-padding]
  return img

## 2
## 2.1 Box filter
### 2.1.1 Kernel 3
# new_img_3 = filter('lenna.png', 'box', 3, 'reflex')
# cv.imshow('Box filter - Kernel 3', new_img_3)

### 2.1.2 Kernel 5
# new_img_5 = filter('lenna.png', 'box', 5, 'constant')
# cv.imshow('Box filter - Kernel 5', new_img_5)

### 2.1.3 Kernel 7
# new_img_7 = filter('lenna.png', 'box', 11, 'edge')
# cv.imshow('Box filter - Kernel 7', new_img_7)

## 2.2 Bartlett filter
### 2.2.1 Kernel 3
# new_img_3 = filter('lenna.png', 'bartlett', 3, 'edge')
# cv.imshow('Bartlett filter - Kernel 3', new_img_3)

### 2.2.2 Kernel 5
# new_img_5 = filter('lenna.png', 'bartlett', 5, 'constant')
# cv.imshow('Bartlett filter - Kernel 5', new_img_5)

### 2.2.3 Kernel 7
# new_img_7 = filter('lenna.png', 'bartlett', 7, 'reflex')
# cv.imshow('Bartlett filter - Kernel 7', new_img_7)

cv.waitKey(0)
cv.destroyAllWindows()
