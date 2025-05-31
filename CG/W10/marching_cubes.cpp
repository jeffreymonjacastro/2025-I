#include "mc_table.h"

#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;

vector<pair<int, int>> edge_connections{{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

struct Vertex3D {
  double x, y, z;
  Vertex3D() : x(0), y(0), z(0) {}
  Vertex3D(double x, double y, double z) : x(x), y(y), z(z) {}

  void write_to_file(fstream &output_file) const {
    output_file << x << " " << y << " " << z << endl;
  }

  Vertex3D operator+(const Vertex3D &other) const {
    return Vertex3D(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Vertex3D operator/(double divisor) const {
    return Vertex3D(this->x / divisor, this->y / divisor, this->z / divisor);
  }
};

struct TriangleFace {
  Vertex3D a, b, c;
  TriangleFace() {}
  TriangleFace(Vertex3D a, Vertex3D b, Vertex3D c) : a(a), b(b), c(c) {}
};

class MarchingCubes {
private:
  string filename;
  vector<TriangleFace> mesh_triangles;
  function<double(double, double, double, double, double, double)> f;
  double grid_height, grid_width, grid_depth;
  int total_vertices = 0;

public:
  MarchingCubes(function<double(double, double, double, double, double, double)> func, string file, double h, double w, double d) : f(func), filename(file), grid_height(h), grid_width(w), grid_depth(d) {}

  void export_file() {
    fstream file(filename, ios::out);
    file << "ply\nformat ascii 1.0\n";
    file << "element vertex " << mesh_triangles.size() * 3 << "\n";
    file << "property float x\nproperty float y\nproperty float z\n";
    file << "element face " << mesh_triangles.size() << "\n";
    file << "property list uchar int vertex_indices\nend_header\n";

    for (const auto &face : mesh_triangles) {
      face.a.write_to_file(file);
      face.b.write_to_file(file);
      face.c.write_to_file(file);
    }

    for (int i = 0; i < mesh_triangles.size(); ++i) {
      file << "3 " << i * 3 << " " << i * 3 + 1 << " " << i * 3 + 2 << endl;
    }

    file.close();
  }

  int calculate_cube_configuration(double x, double y, double z, double dx, double dy, double dz) {
    int config = 0;
    double cx = grid_width / 2, cy = grid_depth / 2, cz = grid_height / 2;

    double v[8][3] = {
        {x, y, z - dz}, {x + dx, y, z - dz}, {x + dx, y + dy, z - dz}, {x, y + dy, z - dz}, {x, y, z}, {x + dx, y, z}, {x + dx, y + dy, z}, {x, y + dy, z}};

    for (int i = 0; i < 8; ++i) {
      if (f(v[i][0], v[i][1], v[i][2], cx, cy, cz) > 0)
        config |= (1 << i);
    }
    return config;
  }

  Vertex3D compute_edge_intersection(Vertex3D p1, Vertex3D p2, double cx, double cy, double cz) {
    double v1 = f(p1.x, p1.y, p1.z, cx, cy, cz);
    double v2 = f(p2.x, p2.y, p2.z, cx, cy, cz);
    if (v1 * v2 > 0)
      return (p1 + p2) / 2.0;

    for (int i = 0; i < 10; ++i) {
      Vertex3D mid = (p1 + p2) / 2.0;
      double vm = f(mid.x, mid.y, mid.z, cx, cy, cz);
      if (abs(vm) < 1e-6)
        break;
      if (v1 * vm < 0) {
        p2 = mid;
        v2 = vm;
      } else {
        p1 = mid;
        v1 = vm;
      }
    }
    return (p1 + p2) / 2.0;
  }

  void create_triangular_mesh(double x, double y, double z, double dx, double dy, double dz, int config) {
    if (config == 0 || config == 255)
      return;

    Vertex3D corners[8] = {
        {x, y, z - dz}, {x + dx, y, z - dz}, {x + dx, y + dy, z - dz}, {x, y + dy, z - dz}, {x, y, z}, {x + dx, y, z}, {x + dx, y + dy, z}, {x, y + dy, z}};

    Vertex3D edges[12];
    for (int i = 0; i < 12; ++i) {
      edges[i] = compute_edge_intersection(corners[edge_connections[i].first], corners[edge_connections[i].second], grid_width / 2, grid_depth / 2, grid_height / 2);
    }

    for (int i = 0; TriangleTable[config][i] != -1; i += 3) {
      TriangleFace tri(edges[TriangleTable[config][i]], edges[TriangleTable[config][i + 1]], edges[TriangleTable[config][i + 2]]);
      mesh_triangles.push_back(tri);
      total_vertices += 3;
    }
  }

  void divide(int depth, double x, double y, double z, double dx, double dy, double dz) {
    if (depth == 0) {
      int config = calculate_cube_configuration(x, y, z, dx, dy, dz);
      create_triangular_mesh(x, y, z, dx, dy, dz, config);
      return;
    }

    double hdx = dx / 2.0, hdy = dy / 2.0, hdz = dz / 2.0;

    divide(depth - 1, x, y, z, hdx, hdy, hdz);
    divide(depth - 1, x + hdx, y, z, hdx, hdy, hdz);
    divide(depth - 1, x, y + hdy, z, hdx, hdy, hdz);
    divide(depth - 1, x + hdx, y + hdy, z, hdx, hdy, hdz);
    divide(depth - 1, x, y, z - hdz, hdx, hdy, hdz);
    divide(depth - 1, x + hdx, y, z - hdz, hdx, hdy, hdz);
    divide(depth - 1, x, y + hdy, z - hdz, hdx, hdy, hdz);
    divide(depth - 1, x + hdx, y + hdy, z - hdz, hdx, hdy, hdz);
  }

  void generate(int depth) {
    divide(depth, 0, 0, grid_height, grid_width, grid_depth, grid_height);
  }
};

int main() {
  auto sphere = [](double x, double y, double z, double cx, double cy, double cz) {
    double dx = x - cx;
    double dy = y - cy;
    double dz = z - cz;
    return dx * dx + dy * dy + dz * dz - 100 * 100;
  };

  auto torus = [](double x, double y, double z, double cx, double cy, double cz) {
    double dx = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) - 50;
    double dz = z - cz;
    return dx * dx + dz * dz - 25 * 25;
  };

  auto complex = [](double x, double y, double z, double cx, double cy, double cz) {
    double dx = x - cx;
    double dy = y - cy;
    double dz = z - cz;
    return dx * dx * dx - 3 * dx * dy * dy - 50000 + dz * dz;
  };

  cout << "Generando archivos PLY..." << endl;

  MarchingCubes sphere_mesh(sphere, "sphere_mesh.ply", 300, 300, 300);
  sphere_mesh.generate(6);
  sphere_mesh.export_file();

  MarchingCubes torus_mesh(torus, "torus_mesh.ply", 300, 300, 300);
  torus_mesh.generate(6);
  torus_mesh.export_file();

  MarchingCubes complex_mesh(complex, "complex_mesh.ply", 300, 300, 300);
  complex_mesh.generate(6);
  complex_mesh.export_file();

  cout << "\nArchivos PLY generados exitosamente!" << endl;
  return 0;
}
