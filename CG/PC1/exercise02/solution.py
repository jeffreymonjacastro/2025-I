import cv2 
import numpy as np

colors = [
  np.array([0, 0, 0]), # black
  np.array([255, 255, 255]),# white
  np.array([0, 0, 255]), # red
  np.array([0, 255, 0]), # green
  np.array([255, 0, 0]), # blue
  np.array([0, 255, 255]), # yellow
]

def chessboard_with_custom_colors(
  WIDTH_IN_PIXELS: int, 
  HEIGHT_IN_PIXELS: int,
  NUM_OF_CELLS_HORIZONTAL: int,
  NUM_OF_CELLS_VERTICAL: int,
  COLORS: list) -> np.ndarray:
  
  pass