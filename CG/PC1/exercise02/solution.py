import cv2 
import numpy as np

colors = [
  np.array([0, 0, 0]), # black
  np.array([255, 255, 255]),# white
  np.array([0, 0, 255]), # red
  np.array([0, 255, 0]), # green
  np.array([255, 0, 0]), # blue
  np.array([0, 255, 255]), # yellow
  np.array([255, 0, 255]), # magenta
  np.array([255, 255, 0]), # cyan
]

def chessboard_with_custom_colors(
  WIDTH_IN_PIXELS: int, 
  HEIGHT_IN_PIXELS: int,
  NUM_OF_CELLS_HORIZONTAL: int,
  NUM_OF_CELLS_VERTICAL: int,
  COLORS: list) -> np.ndarray:
  
  board = np.zeros((HEIGHT_IN_PIXELS, WIDTH_IN_PIXELS, 3), dtype=np.uint8)

  cell_height = HEIGHT_IN_PIXELS // NUM_OF_CELLS_VERTICAL
  cell_width = WIDTH_IN_PIXELS // NUM_OF_CELLS_HORIZONTAL

  for i in range(NUM_OF_CELLS_VERTICAL):
    for j in range(NUM_OF_CELLS_HORIZONTAL):
      color_index = (i + j) % len(COLORS)
      color = COLORS[color_index]

      start_y = i * cell_height
      end_y = (i + 1) * cell_height
      start_x = j * cell_width
      end_x = (j + 1) * cell_width

      board[start_y:end_y, start_x:end_x] = color
  
  return board

result = chessboard_with_custom_colors(400, 400, 8, 8, colors)
cv2.imshow("Chessboard with Custom Colors", result)
cv2.waitKey(0)
cv2.destroyAllWindows()