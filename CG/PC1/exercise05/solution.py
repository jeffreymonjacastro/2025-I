import cv2 
import numpy as np


def box(n: int) -> np.ndarray:
  return np.ones((n, n), np.float32) / (n * n)


def bartlett(n: int) -> np.ndarray:
  if n % 2 == 0:
    raise ValueError("n must be odd")

  l = [x for x in range(1, (n+1)//2)]
  l += [x for x in range((n+1)//2, 0, -1)]  

  m = [[l[i] * l[j] for j in range(n)] for i in range(n)]
  total_sum = np.sum(m) 

  m = np.array(m) / total_sum

  return m.astype(np.float32)


def convolution(img: np.ndarray, kernel: np.ndarray, padding: int):
  width, height = img.shape[:2]

  for i in range(padding, width - padding):
    for j in range(padding, height - padding):
      for c in range(3):  
        region = img[i-padding:i+padding+1, j-padding:j+padding+1, c]
        filtered_value = np.sum(region * kernel)
        img[i, j, c] = filtered_value

  return img.astype(np.uint8)

  
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
    img = convolution(img, kernel, padding)

  elif type_kernel == 'bartlett':
    kernel = bartlett(kernel_size) 
    img = convolution(img, kernel, padding)

  # elif type_kernel == 'gaussian':
  #   kernel = cv2.getGaussianKernel(kernel_size, 0) * cv2.getGaussianKernel(kernel_size, 0).T
  #   img = convolution(img, kernel, padding)

  # elif type_kernel == 'laplacian':
  #   kernel = np.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]], np.float32)
  #   img = convolution(img, kernel, padding)

  img = img[padding:width - padding, padding:height - padding]
  return img.astype(np.uint8)


for i in range(3, 26, 2):
  img = cv2.imread('../lenna.png') 
  filtered_img = filter(img, 'bartlett', i, 'reflex')
  cv2.imwrite(f'output/images/bartlett_{i}.png', filtered_img)
  print(f'Image processed with kernel size {i} and saved as bartlett_{i}.png')


# cv2.waitKey(0)
# cv2.destroyAllWindows()