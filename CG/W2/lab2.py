import cv2 as cv
import numpy as np
from math import *

img = cv.imread('lowcontrast.png')


cv.imshow("Display window", img)
k = cv.waitKey(0)
cv.destroyAllWindows()

