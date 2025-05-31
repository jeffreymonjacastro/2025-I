#include "mc_table.h"

#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

vector<pair<int, int>> edge_connections{{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

double sphere_function(double px, double py, double pz, double center_x, double center_y, double center_z) {
  return pow(px - center_x, 2) + pow(py - center_y, 2) + pow(pz - center_z, 2) - 100 * 100;
}

struct Vertex3D {
  double x, y, z;

  Vertex3D() : x(0), y(0), z(0) {}

  Vertex3D(double x, double y, double z) : x(x), y(y), z(z) {}

  double get_x() const { return x; }
  double get_y() const { return y; }
  double get_z() const { return z; }

  void write_to_file(fstream &output_file) const {
    output_file << x << " " << y << " " << z << endl;
  }

  void display_coordinates() const {
    cout << x << " " << y << " " << z << endl;
  }

  Vertex3D operator+(const Vertex3D &other) const {
    return Vertex3D(this->x + other.get_x(), this->y + other.get_y(), this->z + other.get_z());
  }

  Vertex3D operator/(double divisor) const {
    return Vertex3D(this->x / divisor, this->y / divisor, this->z / divisor);
  }
};

struct TriangleFace {
  Vertex3D vertex_a, vertex_b, vertex_c;

  TriangleFace(Vertex3D first, Vertex3D second, Vertex3D third) : vertex_a(first), vertex_b(second), vertex_c(third) {}

  TriangleFace() {}

  Vertex3D get_vertex_a() const { return vertex_a; }
  Vertex3D get_vertex_b() const { return vertex_b; }
  Vertex3D get_vertex_c() const { return vertex_c; }
};

class MarchingCubes {
private:
  vector<TriangleFace> mesh_triangles;
  vector<int> vertex_indices;
  double grid_height, grid_width, grid_depth;
  int total_vertices;

public:
  MarchingCubes(double h, double w, double d) : grid_height(h), grid_width(w), grid_depth(d) {
    total_vertices = 0;
  }

  void export_file() {
    fstream mesh_file("voxel_mesh.ply", ios::out);
    mesh_file << "ply" << endl;
    mesh_file << "format ascii 1.0" << endl;
    mesh_file << "element vertex " << mesh_triangles.size() * 3 << endl;
    mesh_file << "property float x" << endl;
    mesh_file << "property float y" << endl;
    mesh_file << "property float z" << endl;
    mesh_file << "element face " << mesh_triangles.size() << endl;
    mesh_file << "property list uchar int vertex_indices" << endl;
    mesh_file << "end_header" << endl;

    for (auto &face : mesh_triangles) {
      face.get_vertex_a().write_to_file(mesh_file);
      face.get_vertex_b().write_to_file(mesh_file);
      face.get_vertex_c().write_to_file(mesh_file);
    }

    for (int face_idx = 0; face_idx < mesh_triangles.size(); face_idx++) {
      int idx_a = face_idx * 3;
      int idx_b = face_idx * 3 + 1;
      int idx_c = face_idx * 3 + 2;
      mesh_file << "3 " << idx_a << " " << idx_b << " " << idx_c << endl;
    }

    mesh_file.close();
    cout << "Mesh file exported successfully..." << endl;
    cout << "TOTAL VERTICES GENERATED: " << total_vertices << endl;
  }

  int calculate_cube_configuration(double pos_x, double pos_y, double pos_z, double step_x, double step_y, double step_z) {
    int config_bits = 0;
    double sphere_center_x = grid_width / 2;
    double sphere_center_y = grid_depth / 2;
    double sphere_center_z = grid_height / 2;

    double cube_vertices[8][3] = {
        {pos_x, pos_y, pos_z - step_z},
        {pos_x + step_x, pos_y, pos_z - step_z},
        {pos_x + step_x, pos_y + step_y, pos_z - step_z},
        {pos_x, pos_y + step_y, pos_z - step_z},
        {pos_x, pos_y, pos_z},
        {pos_x + step_x, pos_y, pos_z},
        {pos_x + step_x, pos_y + step_y, pos_z},
        {pos_x, pos_y + step_y, pos_z}};

    for (int vertex_idx = 0; vertex_idx < 8; vertex_idx++) {
      double field_value = sphere_function(cube_vertices[vertex_idx][0], cube_vertices[vertex_idx][1],
                                           cube_vertices[vertex_idx][2], sphere_center_x, sphere_center_y, sphere_center_z);
      if (field_value > 0) {
        config_bits |= (1 << vertex_idx);
      }
    }

    return config_bits;
  }

  Vertex3D compute_edge_intersection(Vertex3D point1, Vertex3D point2, double center_x, double center_y, double center_z) {
    double value1 = sphere_function(point1.get_x(), point1.get_y(), point1.get_z(), center_x, center_y, center_z);
    double value2 = sphere_function(point2.get_x(), point2.get_y(), point2.get_z(), center_x, center_y, center_z);

    if (value1 * value2 > 0) {
      return (point1 + point2) / 2;
    }

    Vertex3D start_point = point1;
    Vertex3D end_point = point2;
    double start_value = value1;
    double end_value = value2;

    for (int iteration = 0; iteration < 10; iteration++) {
      Vertex3D midpoint = (start_point + end_point) / 2;
      double mid_value = sphere_function(midpoint.get_x(), midpoint.get_y(), midpoint.get_z(), center_x, center_y, center_z);

      if (abs(mid_value) < 1e-6)
        break;

      if (start_value * mid_value < 0) {
        end_point = midpoint;
        end_value = mid_value;
      } else {
        start_point = midpoint;
        start_value = mid_value;
      }
    }

    return (start_point + end_point) / 2;
  }

  void create_triangular_mesh(double pos_x, double pos_y, double pos_z, double step_x, double step_y, double step_z, int index) {
    if (index == 0 || index == 255)
      return;

    vector<Vertex3D> voxel_corners;
    for (size_t corner_idx = 0; corner_idx < 8; corner_idx++) {
      switch (corner_idx) {
      case 0:
        voxel_corners.emplace_back(pos_x, pos_y, pos_z - step_z);
        break;
      case 1:
        voxel_corners.emplace_back(pos_x + step_x, pos_y, pos_z - step_z);
        break;
      case 2:
        voxel_corners.emplace_back(pos_x + step_x, pos_y + step_y, pos_z - step_z);
        break;
      case 3:
        voxel_corners.emplace_back(pos_x, pos_y + step_y, pos_z - step_z);
        break;
      case 4:
        voxel_corners.emplace_back(pos_x, pos_y, pos_z);
        break;
      case 5:
        voxel_corners.emplace_back(pos_x + step_x, pos_y, pos_z);
        break;
      case 6:
        voxel_corners.emplace_back(pos_x + step_x, pos_y + step_y, pos_z);
        break;
      case 7:
        voxel_corners.emplace_back(pos_x, pos_y + step_y, pos_z);
        break;
      }
    }

    vector<Vertex3D> edge_intersections(12);
    for (int edge_idx = 0; edge_idx < 12; edge_idx++) {
      Vertex3D corner1 = voxel_corners[edge_connections[edge_idx].first];
      Vertex3D corner2 = voxel_corners[edge_connections[edge_idx].second];
      Vertex3D intersection = compute_edge_intersection(corner1, corner2, grid_width / 2.0, grid_depth / 2.0, grid_height / 2.0);
      edge_intersections[edge_idx] = intersection;
    }

    for (int tri_idx = 0; TriangleTable[index][tri_idx] != -1; tri_idx += 3) {
      if (tri_idx + 2 < 16 && TriangleTable[index][tri_idx + 1] != -1 && TriangleTable[index][tri_idx + 2] != -1) {
        int first_edge = TriangleTable[index][tri_idx];
        int second_edge = TriangleTable[index][tri_idx + 1];
        int third_edge = TriangleTable[index][tri_idx + 2];

        TriangleFace new_face(edge_intersections[first_edge], edge_intersections[second_edge], edge_intersections[third_edge]);
        mesh_triangles.push_back(new_face);
        total_vertices += 3;
      }
    }
  }

  void process_voxel_grid(double subdivision_count) {
    double z_increment = grid_height / subdivision_count;
    double x_increment = grid_width / subdivision_count;
    double y_increment = grid_depth / subdivision_count;

    for (int z_layer = 0; z_layer < subdivision_count; z_layer++) {
      for (int x_index = 0; x_index < subdivision_count; x_index++) {
        for (int y_index = 0; y_index < subdivision_count; y_index++) {
          double current_x = x_index * x_increment;
          double current_y = y_index * y_increment;
          double current_z = grid_height - z_layer * z_increment;

          int cube_config = calculate_cube_configuration(current_x, current_y, current_z, x_increment, y_increment, z_increment);
          create_triangular_mesh(current_x, current_y, current_z, x_increment, y_increment, z_increment, cube_config);
        }
      }
    }
  }
};

int main() {
  MarchingCubes mesh_generator(300, 300, 300);
  mesh_generator.process_voxel_grid(100.0);
  mesh_generator.export_file();
  return 0;
}