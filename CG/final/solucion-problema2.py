import cv2
import numpy as np
import time
from collections import deque

def solve_maze(path_input_image: str, path_output_image: str):
    start_time = time.time()
    # Leer imagen
    image = cv2.imread(path_input_image)
    if image is None:
        raise ValueError(f"No se pudo leer la imagen: {path_input_image}")

    # Convertir a escala de grises
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Umbral para distinguir paredes (oscuras) y caminos (claros)
    wall_threshold = 80  # Puedes ajustar este valor según el laberinto
    binary = (gray > wall_threshold).astype(np.uint8)

    height, width = binary.shape

    # Buscar entrada (primer píxel blanco en el borde superior o izquierdo)
    entrance = None
    for x in range(width):
        if binary[0, x] == 1:
            entrance = (0, x)
            break
    if entrance is None:
        for y in range(height):
            if binary[y, 0] == 1:
                entrance = (y, 0)
                break

    # Buscar salida (primer píxel blanco en el borde inferior o derecho)
    exit_point = None
    for x in range(width):
        if binary[height-1, x] == 1:
            exit_point = (height-1, x)
            break
    if exit_point is None:
        for y in range(height):
            if binary[y, width-1] == 1:
                exit_point = (y, width-1)
                break

    if entrance is None or exit_point is None:
        raise ValueError("No se encontraron puntos de entrada o salida válidos")

    # BFS para encontrar el camino más corto
    path = bfs_shortest_path(binary, entrance, exit_point)
    if path is None:
        raise ValueError("No se encontró un camino válido entre entrada y salida")

    # Dibujar el camino en rojo sobre la imagen en escala de grises
    output_bgr = cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)
    for y, x in path:
        output_bgr[y, x] = [0, 0, 255]  # Rojo en BGR

    cv2.imwrite(path_output_image, output_bgr)

    elapsed_time = time.time() - start_time
    if elapsed_time > 10:
        print(f"Advertencia: El algoritmo tardó {elapsed_time:.2f} segundos")
    else:
        print(f"Tiempo de ejecución: {elapsed_time:.2f} segundos")
    print(f"Imagen guardada en: {path_output_image}")

def bfs_shortest_path(binary, start, end):
    height, width = binary.shape
    directions = [(-1,0), (1,0), (0,-1), (0,1)]
    queue = deque([(start[0], start[1], [start])])
    visited = set()
    visited.add(start)
    while queue:
        y, x, path = queue.popleft()
        if (y, x) == end:
            return path
        for dy, dx in directions:
            ny, nx = y + dy, x + dx
            if 0 <= ny < height and 0 <= nx < width:
                if binary[ny, nx] == 1 and (ny, nx) not in visited:
                    visited.add((ny, nx))
                    queue.append((ny, nx, path + [(ny, nx)]))
    return None

if __name__ == "__main__":
    # Cambia los nombres de archivo según corresponda
    solve_maze("maze.png", "maze_solved.png")