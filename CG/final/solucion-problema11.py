import numpy as np
from PIL import Image
import random

def generate_maze(
    width_in_pixels: int,
    height_in_pixels: int,
    wall_thickness_in_pixels: int,
    number_of_walls: int,  # Este parámetro no se usa directamente en DFS
    wall_color=(0, 0, 0),  # Color de las paredes (por defecto negro)
    path_color=(255, 255, 255),  # Color de los caminos (por defecto blanco)
):
    """
    Genera una imagen de un laberinto perfectamente conexo usando DFS (backtracking).
    - Solo dos aberturas en los bordes (entrada y salida).
    - Las paredes paralelas solo se tocan en un segmento igual al grosor de la pared.
    - Escalable a imágenes grandes.
    - Ajusta el tamaño real para evitar bordes blancos.
    - Permite personalizar el color de paredes y caminos.

    Args:
        width_in_pixels (int): Ancho de la imagen en píxeles.
        height_in_pixels (int): Alto de la imagen en píxeles.
        wall_thickness_in_pixels (int): Grosor de las paredes en píxeles.
        number_of_walls (int): (No usado en este método, pero se mantiene para compatibilidad)
        wall_color (tuple): Color RGB de las paredes.
        path_color (tuple): Color RGB de los caminos.

    Returns:
        PIL.Image.Image: Imagen del laberinto.
    """
    # --- 0. Ajuste de dimensiones para evitar bordes blancos ---
    # El tamaño debe ser múltiplo del grosor de la pared y el número de celdas debe ser impar
    def ajustar_dimension(dimension, wall_thickness):
        # Asegura que la dimensión sea múltiplo del grosor de la pared
        if dimension % wall_thickness != 0:
            dimension += wall_thickness - (dimension % wall_thickness)
        # Calcula el número de celdas posibles
        n_cells = (dimension - wall_thickness) // (2 * wall_thickness)
        # Si es par, suma una celda más (agregando 2*wall_thickness)
        if n_cells % 2 == 0:
            dimension += 2 * wall_thickness
            n_cells += 1
        return dimension, n_cells

    width_in_pixels, n_cells_x = ajustar_dimension(width_in_pixels, wall_thickness_in_pixels)
    height_in_pixels, n_cells_y = ajustar_dimension(height_in_pixels, wall_thickness_in_pixels)

    cell_size = wall_thickness_in_pixels * 2  # Celda + pared a cada lado
    real_width = n_cells_x * cell_size + wall_thickness_in_pixels
    real_height = n_cells_y * cell_size + wall_thickness_in_pixels

    # --- 2. Inicializar la matriz del laberinto ---
    # 0 = camino (blanco), 1 = pared (negro)
    maze = np.ones((real_height, real_width), dtype=np.uint8)
    
    # Rellenar celdas con caminos
    for y in range(n_cells_y):
        for x in range(n_cells_x):
            y0 = y * cell_size + wall_thickness_in_pixels
            x0 = x * cell_size + wall_thickness_in_pixels
            maze[y0 : y0 + wall_thickness_in_pixels, x0 : x0 + wall_thickness_in_pixels] = 0

    # --- 3. DFS para crear el laberinto ---
    visited = np.zeros((n_cells_y, n_cells_x), dtype=bool)
    stack = []
    start_cell = (0, 0)
    stack.append(start_cell)
    visited[0, 0] = True
    directions = [(-1,0),(1,0),(0,-1),(0,1)]  # Arriba, abajo, izq, der
    random.shuffle(directions)

    while stack:
        cy, cx = stack[-1]
        neighbors = []
        for dy, dx in directions:
            ny, nx = cy + dy, cx + dx
            if 0 <= ny < n_cells_y and 0 <= nx < n_cells_x and not visited[ny, nx]:
                neighbors.append((ny, nx, dy, dx))
        if neighbors:
            ny, nx, dy, dx = random.choice(neighbors)
            # Abrir camino entre (cy,cx) y (ny,nx)
            y0 = cy * cell_size + wall_thickness_in_pixels
            x0 = cx * cell_size + wall_thickness_in_pixels
            y1 = ny * cell_size + wall_thickness_in_pixels
            x1 = nx * cell_size + wall_thickness_in_pixels
            if dy == 1:  # abajo
                maze[y0 + wall_thickness_in_pixels : y1, x0 : x0 + wall_thickness_in_pixels] = 0
            elif dy == -1:  # arriba
                maze[y1 + wall_thickness_in_pixels : y0, x0 : x0 + wall_thickness_in_pixels] = 0
            elif dx == 1:  # derecha
                maze[y0 : y0 + wall_thickness_in_pixels, x0 + wall_thickness_in_pixels : x1] = 0
            elif dx == -1:  # izquierda
                maze[y0 : y0 + wall_thickness_in_pixels, x1 + wall_thickness_in_pixels : x0] = 0
            visited[ny, nx] = True
            stack.append((ny, nx))
        else:
            stack.pop()

    # --- 4. Crear entrada y salida en lados opuestos ---
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

    # --- 5. Convertir la matriz a imagen Pillow con colores personalizados ---
    # Creamos una imagen RGB y asignamos colores según el valor de la matriz
    img_rgb = np.zeros((real_height, real_width, 3), dtype=np.uint8)
    img_rgb[maze == 1] = wall_color  # Paredes
    img_rgb[maze == 0] = path_color  # Caminos
    img = Image.fromarray(img_rgb)
    return img

if __name__ == "__main__":
    # Parámetros
    WIDTH = 400
    HEIGHT = 700
    WALL_THICKNESS = 20
    NUM_WALLS = 300  # No usado, pero se mantiene
    WALL_COLOR = (6, 64, 43)         
    PATH_COLOR = (155, 161, 157)  

    print("Generando laberinto... Maze generating... ⏳")
    maze_image = generate_maze(
        width_in_pixels=WIDTH,
        height_in_pixels=HEIGHT,
        wall_thickness_in_pixels=WALL_THICKNESS,
        number_of_walls=NUM_WALLS,
        wall_color=WALL_COLOR,
        path_color=PATH_COLOR,
    )
    output_filename = "maze_generated.png"
    maze_image.save(output_filename)
    print(f"¡Laberinto guardado como '{output_filename}'! ✅")
    # maze_image.show()  # Descomenta para ver la imagen