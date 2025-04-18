import cv2 
import numpy as np
from math import sqrt
import random


def bouncing_animation(height, width, radius, n):
  centers = []
  while len(centers) < n:
    new_center = (random.randint(radius, width - radius), random.randint(radius, height - radius))
    if all(sqrt((new_center[0] - c[0])**2 + (new_center[1] - c[1])**2) > 2 * radius for c in centers):
      centers.append(new_center)

  velocities = [(random.choice([-10, 5]), random.choice([-3, 3])) for _ in range(n)]
  colors = [tuple(random.choices(range(256), k=3)) for _ in range(n)]

  while True:
    canvas = np.zeros((height, width, 3), dtype=np.uint8)

    for i in range(n):
      centers[i] = (centers[i][0] + velocities[i][0], centers[i][1] + velocities[i][1])

      if centers[i][0] + radius >= width or centers[i][0] - radius <= 0:
        velocities[i] = (-velocities[i][0], velocities[i][1])
      if centers[i][1] + radius >= height or centers[i][1] - radius <= 0:
        velocities[i] = (velocities[i][0], -velocities[i][1])

    for i in range(n):
      for j in range(i + 1, n):
        dx = centers[i][0] - centers[j][0]
        dy = centers[i][1] - centers[j][1]
        distance = sqrt(dx**2 + dy**2)

        if distance <= 2 * radius:
          velocities[i], velocities[j] = velocities[j], velocities[i]

    for i in range(n):
      cv2.circle(canvas, centers[i], radius, colors[i], -1)

    cv2.imshow("Bouncing Balls", canvas)

    if cv2.waitKey(5) & 0xFF == ord('q'):
      break

  cv2.destroyAllWindows()

bouncing_animation(700, 1000, 30, 5)