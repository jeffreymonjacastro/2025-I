import cv2 as cv
import numpy as np
from math import *
from random import randint

# IMAGES
## 1.1
def chessboard(h_px, w_px, nh, nw):
  board = np.zeros((h_px, w_px), dtype=np.uint8)

  h_celda = h_px // nh
  w_celda = w_px // nw

  for i in range(nh):
    for j in range(nw):
      color = 255 if (i + j) % 2 == 0 else 0

      start_y = i * h_celda
      end_y = (i + 1) * h_celda
      start_x = j * w_celda
      end_x = (j + 1) * w_celda

      board[start_y:end_y, start_x:end_x] = color

  return board

# cv.imshow("Chessboard", chessboard(400, 400, 8, 8))
# cv.waitKey(100)

## 1.2
def multicolorboard(h_px, w_px, nh, nw):
  board = np.zeros((h_px, w_px, 3), dtype=np.uint8)

  h_celda = h_px // nh
  w_celda = w_px // nw

  for i in range(nh):
    for j in range(nw):
      r = randint(0, 255)
      g = randint(0, 255)
      b = randint(0, 255)
      color = [r, g, b] if (i + j) % 2 == 0 else [r, g, b]

      start_y = i * h_celda
      end_y = (i + 1) * h_celda
      start_x = j * w_celda
      end_x = (j + 1) * w_celda

      board[start_y:end_y, start_x:end_x] = color

  return board

# cv.imshow("Multicolor Board", multicolorboard(400, 400, 8, 8))
# cv.waitKey(100)

## 1.3
def generate_circle(canvas_size, center_coordinates, radius, color, thickness):
  canvas = np.ones((*canvas_size, 3), dtype=np.uint8) * 255
  image = cv.circle(canvas, center_coordinates, radius, color, thickness)
  return image

# img = circlecv2((400,400),(200, 200), 100, (0,0,255), -1)
# cv.imshow("Circle", img)
# k = cv.waitKey(0)
# cv.destroyAllWindows()

## 1.4
# Fractal de Maldebrot

# ANIMATIONS
## 2.1
def bouncing_animation(height, width, radius):
  canvas = np.ones((height, width, 3), dtype=np.uint8) * 255
  center_red = (width // 4, height // 4)
  center_blue = (width // 2, height // 2)
  vx_red = 5
  vy_red = 3
  vx_blue = -5
  vy_blue = -3

  while True:
    canvas = np.ones((height, width, 3), dtype=np.uint8) * 255
    center_red = (center_red[0] + vx_red, center_red[1] + vy_red)
    center_blue = (center_blue[0] + vx_blue, center_blue[1] + vy_blue)

    # Wall collision detection
    if center_red[0] + radius >= width or center_red[0] - radius <= 0:
      vx_red = -vx_red
    if center_red[1] + radius >= height or center_red[1] - radius <= 0:
      vy_red = -vy_red
    if center_blue[0] + radius >= width or center_blue[0] - radius <= 0:
      vx_blue = -vx_blue
    if center_blue[1] + radius >= height or center_blue[1] - radius <= 0:
      vy_blue = -vy_blue

    # Circle collision detection
    dx = center_red[0] - center_blue[0]
    dy = center_red[1] - center_blue[1]
    distance = sqrt(dx**2 + dy**2)

    if distance <= 2 * radius:
      # Reverse velocities upon collision
      vx_red, vx_blue = vx_blue, vx_red
      vy_red, vy_blue = vy_blue, vy_red

    # Draw circles
    cv.circle(canvas, center_red, radius, (0, 0, 255), -1)
    cv.circle(canvas, center_blue, radius, (255, 0, 0), -1)
    cv.imshow("Bouncing Circle", canvas)

    if cv.waitKey(10) & 0xFF == ord('q'): 
      break

  cv.destroyAllWindows()

bouncing_animation(400, 400, 30)

## 2.2 
## Game Life in a board of size NxM

