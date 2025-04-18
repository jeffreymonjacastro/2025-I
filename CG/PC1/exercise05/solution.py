import cv2 
import numpy as np


def box(n: int) -> np.ndarray:
  if n % 2 == 0:
    raise ValueError("Kernel size must be odd")

  kernel = np.ones((n, n), np.float32) / (n * n)
  return kernel.astype(np.float32)


def bartlett(n: int) -> np.ndarray:
  if n % 2 == 0:
    raise ValueError("Kernel size must be odd")

  l = [x for x in range(1, (n+1)//2)]
  l += [x for x in range((n+1)//2, 0, -1)]  

  kernel = [[l[i] * l[j] for j in range(n)] for i in range(n)]
  total_sum = np.sum(kernel) 

  kernel = np.array(kernel) / total_sum

  return kernel.astype(np.float32)


def gaussian(n: int) -> np.ndarray:
  if n % 2 == 0:
    raise ValueError("Kernel size must be odd")

  coef = [1]
  for i in range(1, n):
    new_row = [1]
    for j in range(1, i):
      new_row.append(coef[j-1] + coef[j])
    new_row.append(1)
    coef = new_row

  kernel = [[coef[i] * coef[j] for j in range(n)] for i in range(n)]
  total_sum = np.sum(kernel)

  kernel = np.array(kernel) / total_sum

  return kernel.astype(np.float32)


def laplacian(n: int) -> np.ndarray:
  if n % 2 == 0:
    raise ValueError("Kernel size must be odd")

  if n == 3:
    kernel = np.array(
      [[0, 1, 0], 
      [1, -4, 1], 
      [0, 1, 0]], np.float32)
  elif n == 5:
    kernel = np.array(
      [[0, 0, 1, 0, 0], 
      [0, 1, 2, 1, 0], 
      [1, 2, -17, 2, 1], 
      [0, 1, 2, 1, 0], 
      [0, 0, 1, 0, 0]], np.float32)
  else:
    raise ValueError("Kernel size must be 3 or 5")

  return kernel.astype(np.float32)

def convolution(img: np.ndarray, kernel: np.ndarray, padding: int):
  width, height, canals = img.shape
  out = np.zeros((width, height, canals), np.float32)

  for i in range(padding, width - padding):
    for j in range(padding, height - padding):
      for k in range(canals):  
        region = img[i-padding:i+padding+1, j-padding:j+padding+1, k]
        filtered_value = np.sum(region * kernel)
        out[i, j, k] = filtered_value
      
  out = np.clip(out, 0, 255).astype(np.uint8)  
  return out

  
def filter(
  img: np.ndarray, 
  type_kernel: str, 
  kernel_size: int, 
  padding_strategy: str) -> np.ndarray:

  width, height = img.shape[:2]
  padding = kernel_size // 2

  # Paddings
  if padding_strategy == 'constant':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='constant')
  elif padding_strategy == 'edge':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='edge')
  elif padding_strategy == 'reflex':
    img = np.pad(img, pad_width=((padding, padding), (padding, padding), (0, 0)), mode='reflect')

  # Kernels
  if type_kernel == 'box':
    kernel = box(kernel_size) 
  elif type_kernel == 'bartlett':
    kernel = bartlett(kernel_size) 
  elif type_kernel == 'gaussian':
    kernel = gaussian(kernel_size)
  elif type_kernel == 'laplacian':
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    kernel = laplacian(kernel_size)

  img = convolution(img, kernel, padding)

  img = img[padding:width - padding, padding:height - padding]
  return img.astype(np.uint8)


# for i in range(3, 26, 2):
#   img = cv2.imread('../lenna.png') 
#   filtered_img = filter(img, 'gaussian', i, 'reflex')
#   cv2.imwrite(f'output/images/gaussian_{i}.png', filtered_img)
#   print(f'Image processed with kernel size {i} and saved as gaussian_{i}.png')

# img = cv2.imread('../lenna.png')
# filtered_img = filter(img, 'laplacian', 5, 'edge')
# cv2.imwrite('output/images/laplacian_5.png', filtered_img)

# cv2.imshow('Laplacian Filter', filtered_img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()