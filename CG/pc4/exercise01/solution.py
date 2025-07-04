import numpy as np
from PIL import Image
import math

def read_ply(filename):
    vertex = []
    color = []

    with open(filename, 'r') as f:
        n = 0

        while True:
            l = f.readline().strip()
            if l.startswith("element vertex"):
                n = int(l.split()[-1])
            elif l == "end_header":
                break
        
        for _ in range(n):
            row = f.readline().strip().split()
            x, y, z, r, g, b = row
            vertex.append([float(x), float(y), float(z)])
            color.append([int(r), int(g), int(b)])

    vertex = np.array(vertex, dtype=np.float32)
    color = np.array(color, dtype=np.uint8)
    return vertex, color


def project_points(
    full_path_input_mesh: str,
    optical_center_x: float,
    optical_center_y: float,
    optical_center_z: float,
    optical_axis_x: float,
    optical_axis_y: float,
    optical_axis_z: float,
    up_vector_x: float,
    up_vector_y: float,
    up_vector_z: float,
    focal_distance: float,
    output_width_in_pixels: int,
    output_height_in_pixels: int,
    full_path_output: str
):
    vertex, colors = read_ply(full_path_input_mesh)

    camera_center = np.array([
        optical_center_x, 
        optical_center_y, 
        optical_center_z
    ], dtype=np.float32)

    optical_direction = np.array([
        optical_axis_x, 
        optical_axis_y, 
        optical_axis_z
    ], dtype=np.float32)

    up_direction = np.array([
        up_vector_x, 
        up_vector_y, 
        up_vector_z
    ], dtype=np.float32)

    optical_direction = optical_direction / np.linalg.norm(optical_direction)
    up_direction = up_direction / np.linalg.norm(up_direction)

    z_axis = -optical_direction
    
    x_axis = np.cross(up_direction, z_axis)
    
    if np.linalg.norm(x_axis) < 1e-6:
        if abs(z_axis[0]) < 0.9:
            alternative_up = np.array([1.0, 0.0, 0.0])
        else:
            alternative_up = np.array([0.0, 1.0, 0.0])
        x_axis = np.cross(alternative_up, z_axis)
    
    x_axis = x_axis / np.linalg.norm(x_axis)
    
    y_axis = np.cross(z_axis, x_axis)
    y_axis = y_axis / np.linalg.norm(y_axis)

    rotation_matrix = np.array([x_axis, y_axis, z_axis], dtype=np.float32)

    camera_vertices = (vertex - camera_center) @ rotation_matrix.T

    visible_mask = camera_vertices[:, 2] < 0
    visible_vertices = camera_vertices[visible_mask]

    if colors is not None:
        visible_colors = colors[visible_mask]
    else:
        visible_colors = None

    if len(visible_vertices) == 0:
        img = Image.new('RGB', (output_width_in_pixels, output_height_in_pixels), (0, 0, 0))
        img.save(full_path_output)
        return

    projected_x = (visible_vertices[:, 0] * focal_distance) / (-visible_vertices[:, 2])
    projected_y = (visible_vertices[:, 1] * focal_distance) / (-visible_vertices[:, 2])

    center_x = output_width_in_pixels / 2.0
    center_y = output_height_in_pixels / 2.0

    pixel_u = projected_x + center_x
    pixel_v = -projected_y + center_y  

    output_image = np.zeros((output_height_in_pixels, output_width_in_pixels, 3), dtype=np.uint8)
    z_buffer = np.full((output_height_in_pixels, output_width_in_pixels), float('inf'))

    for i in range(len(pixel_u)):
        screen_x = int(round(pixel_u[i]))
        screen_y = int(round(pixel_v[i]))

        if 0 <= screen_x < output_width_in_pixels and 0 <= screen_y < output_height_in_pixels:
            depth = -visible_vertices[i, 2]  
            
            if depth < z_buffer[screen_y, screen_x]:
                z_buffer[screen_y, screen_x] = depth
                
                if visible_colors is not None:
                    pixel_color = visible_colors[i]
                else:
                    pixel_color = [255, 255, 255]  

                output_image[screen_y, screen_x] = pixel_color

    final_image = Image.fromarray(output_image)
    final_image.save(full_path_output)

    
project_points(
    full_path_input_mesh='sphere.ply',
    optical_center_x=1.5,
    optical_center_y=1.5,
    optical_center_z=1.5,
    optical_axis_x=-1.0,
    optical_axis_y=-1.0,
    optical_axis_z=-1.0,
    up_vector_x=0.0,
    up_vector_y=1.0,
    up_vector_z=0.0,
    focal_distance=400.0,
    output_width_in_pixels=640,
    output_height_in_pixels=480,
    full_path_output="prueba.png"
)


