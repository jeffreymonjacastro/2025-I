import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

img1 = cv.imread('./img/utec1.jpg', cv.IMREAD_GRAYSCALE)  
img2 = cv.imread('./img/utec3.jpg', cv.IMREAD_GRAYSCALE)  

# Initialize SIFT detector
sift = cv.SIFT_create()

# find the keypoints and descriptors with SIFT
kp1, des1 = sift.detectAndCompute(img1,None)
kp2, des2 = sift.detectAndCompute(img2,None)

# BFMatcher with default params
bf = cv.BFMatcher()
matches = bf.knnMatch(des1,des2,k=2)

# Apply ratio test
good = []
for m,n in matches:
    if m.distance < 0.6*n.distance:
        good.append([m])

# img=cv.drawKeypoints(gray,kp,img)
# img=cv.drawKeypoints(gray,kp,img,flags=cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# cv.drawMatchesKnn expects list of lists as matches.
# Definir color para los matches (BGR format) - rojo en este caso
matchColor = (0, 0, 255)  
singlePointColor = (255, 0, 0)  

img3 = cv.drawMatchesKnn(img1,kp1,img2,kp2,good,None,
                        matchColor=matchColor,
                        singlePointColor=singlePointColor,
                        flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

# plt.imshow(img3),plt.show()
cv.imwrite('sift_matches.jpg',img3)


