#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class generatePLY {
private:
  ofstream file;

public:
  generatePLY(string const &s) : file(s) {
    if (!file.is_open()) {
      cerr << "Error opening file: " << s << endl;
      exit(EXIT_FAILURE);
    }

    header();
  }

  void header() {
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex 8\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "element face 6\n";
    file << "property list uchar int vertex_index\n";
    file << "end_header\n";
  }

  void printCube(vector<double> const &vertices) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
      file << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << "\n";
    }

    vector<vector<int>> faces = {
        {3, 2, 1, 0},
        {4, 5, 6, 7},
        {0, 1, 5, 4},
        {2, 3, 7, 6},
        {4, 7, 3, 0},
        {1, 2, 6, 5}};

    for (const auto &face : faces) {
      file << "4 ";
      for (int vertex_index : face) {
        file << vertex_index << " ";
      }
      file << "\n";
    }

    file.close();
  }
};

void cube_with_square_faces(double min_x, double max_x, double min_y, double max_y, double min_z, double max_z, string const &full_path_output_file) {
  generatePLY ply(full_path_output_file);

  vector<double> vertices = {
      min_x, min_y, min_z,
      max_x, min_y, min_z,
      max_x, max_y, min_z,
      min_x, max_y, min_z,
      min_x, min_y, max_z,
      max_x, min_y, max_z,
      max_x, max_y, max_z,
      min_x, max_y, max_z};

  ply.printCube(vertices);
}

int main() {
  cube_with_square_faces(1.0, 2.0, 1.0, 2.0, 1.0, 2.0, "cube.ply");
  return 0;
}
