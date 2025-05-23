#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
  double x, y, z;
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

vector<Point> tetrahedron_vertices = {
  Point p0(1.0, 0.0, 0.0);
  Point p1(-1/3, 2*sqrt(2)/3, 0);
  Point p2(-1/3, -sqrt(2)/3, sqrt(6)/3);
  Point p3(-1/3, -sqrt(2)/3, -sqrt(6)/3);
};

vector<vector<int>> tetrahedron_faces = {
  {0, 1, 2},
  {0, 1, 3},
  {0, 2, 3},
  {1, 2, 3}
};

void subdivision(Tetrahedron tetraedro){
  vector<Point> new_vertices;
  vector<vector<int>> new_faces;

  // Calculate the midpoints of each edge
  for (const auto& face : tetrahedron_faces) {
    for (int i = 0; i < face.size(); ++i) {
      int v1 = face[i];
      int v2 = face[(i + 1) % face.size()];
      Point midpoint(
        (tetrahedron_vertices[v1].x + tetrahedron_vertices[v2].x) / 2,
        (tetrahedron_vertices[v1].y + tetrahedron_vertices[v2].y) / 2,
        (tetrahedron_vertices[v1].z + tetrahedron_vertices[v2].z) / 2
      );
      new_vertices.push_back(midpoint);
    }
  }

  // Create new faces using the midpoints and original vertices
  for (const auto& face : tetrahedron_faces) {
    int n = face.size();
    for (int i = 0; i < n; ++i) {
      int v1 = face[i];
      int v2 = face[(i + 1) % n];
      int m1 = new_vertices.size() - n + i;
      int m2 = new_vertices.size() - n + (i + 1) % n;
      new_faces.push_back({v1, m1, m2});
    }
  }

  // Add the new vertices and faces to the original tetrahedron
  tetrahedron_vertices.insert(tetrahedron_vertices.end(), new_vertices.begin(), new_vertices.end());
  tetrahedron_faces.insert(tetrahedron_faces.end(), new_faces.begin(), new_faces.end());
};

int main(){


  return 0;
}

