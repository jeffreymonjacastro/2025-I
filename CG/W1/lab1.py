import cv2 as cv
import numpy as np
from math import *

# print(img)
img = cv.imread('Lenna.png')

def bilinear_interpolation(A, new_shape):
	old_height, old_width = A.shape[:2]
	new_height, new_width = new_shape

	B = np.zeros((new_height, new_width, 3), dtype=np.uint8)

	scale_height = (old_height - 1) / (new_height - 1)
	scale_width = (old_width - 1) / (new_width - 1)

	for i in range(new_height):
		for j in range(new_width):
			x = i * scale_height
			y = j * scale_width

			x0, x1 = int(np.floor(x)), int(np.ceil(x))
			if x1 >= old_height: x1 = old_height - 1

			y0, y1 = int(np.floor(y)), int(np.ceil(y))
			if y1 >= old_width: y1 = old_width - 1
			
			# 4 puntos alrededor del punto (x, y)
			f00 = A[x0, y0]
			f01 = A[x0, y1]
			f10 = A[x1, y0]
			f11 = A[x1, y1]

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

			# print("Pixel (" + str(i) + ", " + str(j) + "): ", "x: ", round(x,1), " | y: ", round(y,1), " | x0: ", x0, " | x1: ", x1, " | y0: ", y0, " | y1: ", y1, " | f00: ", f00, " | f01: ", f01, " | f10: ", f10, " | f11: ", f11, " | f0: ", f0, " | f1: ", f1, " | f: ", f)
			
			B[i, j] = np.clip(f, 0, 255).astype(np.uint8) 

			#print("f: ", f, "B: ", B[i, j])
	return B

new_img = bilinear_interpolation(img, (500, 500))
# print(new_img)
cv.imshow("Display window", new_img)

k = cv.waitKey(0)
