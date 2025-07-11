import numpy as np
from PIL import Image
import random

def generate_maze(
    width_in_pixels: int,
    height_in_pixels: int,
    wall_thickness_in_pixels: int,
    number_of_walls: int, 
    wall_color=(0, 0, 0),
    path_color=(255, 255, 255),
):
    """
    Genera una imagen de un laberinto usando DFS (backtracking), donde number_of_walls influye en la cantidad de bifurcaciones:
    - Si number_of_walls es bajo, los caminos tienden a ser más rectos (menos bifurcaciones).
    - Si es alto, hay más bifurcaciones y caminos más cortos.
    - El DFS recorre todas las celdas (no hay islas).
    """

    def ajustar_dimension(dimension, wall_thickness):
        if dimension % wall_thickness != 0:
            dimension += wall_thickness - (dimension % wall_thickness)
        n_cells = (dimension - wall_thickness) // (2 * wall_thickness)
        if n_cells % 2 == 0:
            dimension += 2 * wall_thickness
            n_cells += 1
        return dimension, n_cells

    width_in_pixels, n_cells_x = ajustar_dimension(width_in_pixels, wall_thickness_in_pixels)
    height_in_pixels, n_cells_y = ajustar_dimension(height_in_pixels, wall_thickness_in_pixels)

    cell_size = wall_thickness_in_pixels * 2
    real_width = n_cells_x * cell_size + wall_thickness_in_pixels
    real_height = n_cells_y * cell_size + wall_thickness_in_pixels

    maze = np.ones((real_height, real_width), dtype=np.uint8)
    for y in range(n_cells_y):
        for x in range(n_cells_x):
            y0 = y * cell_size + wall_thickness_in_pixels
            x0 = x * cell_size + wall_thickness_in_pixels
            maze[y0 : y0 + wall_thickness_in_pixels, x0 : x0 + wall_thickness_in_pixels] = 0

    visited = np.zeros((n_cells_y, n_cells_x), dtype=bool)
    stack = []
    start_cell = (0, 0)
    stack.append((start_cell, None)) 
    visited[0, 0] = True
    directions = [(-1,0),(1,0),(0,-1),(0,1)]  

    total_cells = n_cells_x * n_cells_y
    min_walls = max(1, total_cells // 8) 
    max_walls = total_cells * 2          
    nw = max(min_walls, min(number_of_walls, max_walls))
    p_straight = 1.0 - (nw - min_walls) / (max_walls - min_walls + 1e-9) * 0.85  # entre 1.0 y 0.15 aprox

    while stack:
        (cy, cx), prev_dir = stack[-1]
        neighbors = []
        for idx, (dy, dx) in enumerate(directions):
            ny, nx = cy + dy, cx + dx
            if 0 <= ny < n_cells_y and 0 <= nx < n_cells_x and not visited[ny, nx]:
                neighbors.append((ny, nx, dy, dx, idx))
        if neighbors:
            # Elegir si seguimos recto o cambiamos de dirección
            if prev_dir is not None:
                # Buscar el vecino en la misma dirección
                straight = [n for n in neighbors if n[2:4] == prev_dir]
                if straight and random.random() < p_straight:
                    chosen = straight[0]
                else:
                    chosen = random.choice(neighbors)
            else:
                chosen = random.choice(neighbors)
            ny, nx, dy, dx, idx = chosen
            y0 = cy * cell_size + wall_thickness_in_pixels
            x0 = cx * cell_size + wall_thickness_in_pixels
            y1 = ny * cell_size + wall_thickness_in_pixels
            x1 = nx * cell_size + wall_thickness_in_pixels
            if dy == 1:
                maze[y0 + wall_thickness_in_pixels : y1, x0 : x0 + wall_thickness_in_pixels] = 0
            elif dy == -1:
                maze[y1 + wall_thickness_in_pixels : y0, x0 : x0 + wall_thickness_in_pixels] = 0
            elif dx == 1:
                maze[y0 : y0 + wall_thickness_in_pixels, x0 + wall_thickness_in_pixels : x1] = 0
            elif dx == -1:
                maze[y0 : y0 + wall_thickness_in_pixels, x1 + wall_thickness_in_pixels : x0] = 0
            visited[ny, nx] = True
            stack.append(((ny, nx), (dy, dx)))
        else:
            stack.pop()

    sides = [("top", 0), ("bottom", real_height-1), ("left", 0), ("right", real_width-1)]
    entry_side, exit_side = random.sample(sides, 2)
    def open_on_side(side, maze):
        if side[0] == "top":
            x = random.randint(0, n_cells_x-1)
            x0 = x * cell_size + wall_thickness_in_pixels
            maze[0:wall_thickness_in_pixels, x0:x0+wall_thickness_in_pixels] = 0
        elif side[0] == "bottom":
            x = random.randint(0, n_cells_x-1)
            x0 = x * cell_size + wall_thickness_in_pixels
            maze[-wall_thickness_in_pixels:, x0:x0+wall_thickness_in_pixels] = 0
        elif side[0] == "left":
            y = random.randint(0, n_cells_y-1)
            y0 = y * cell_size + wall_thickness_in_pixels
            maze[y0:y0+wall_thickness_in_pixels, 0:wall_thickness_in_pixels] = 0
        elif side[0] == "right":
            y = random.randint(0, n_cells_y-1)
            y0 = y * cell_size + wall_thickness_in_pixels
            maze[y0:y0+wall_thickness_in_pixels, -wall_thickness_in_pixels:] = 0
    open_on_side(entry_side, maze)
    open_on_side(exit_side, maze)

    img_rgb = np.zeros((real_height, real_width, 3), dtype=np.uint8)
    img_rgb[maze == 1] = wall_color
    img_rgb[maze == 0] = path_color
    img = Image.fromarray(img_rgb)
    return img

if __name__ == "__main__":
    WIDTH = 100
    HEIGHT = 200
    WALL_THICKNESS = 10
    NUM_WALLS = 1000
    WALL_COLOR = (0, 0, 0)
    PATH_COLOR = (255, 155, 255)

    print("Generando laberinto... Maze generating... ⏳")
    maze_image = generate_maze(
        width_in_pixels=WIDTH,
        height_in_pixels=HEIGHT,
        wall_thickness_in_pixels=WALL_THICKNESS,
        number_of_walls=NUM_WALLS,
        wall_color=WALL_COLOR,
        path_color=PATH_COLOR,
    )
    output_filename = "maze.png"
    maze_image.save(output_filename)
    print(f"¡Laberinto guardado como '{output_filename}'! ✅")