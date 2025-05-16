#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <vector>

using namespace std;

#define PI 3.14159265358979323846

struct Point {
  double x;
  double y;
  double z;
};

vector<vector<Point>> generate_sphere(int paralelos, int meridianos, int r) {
  vector<vector<Point>> sphere;

  for (int i = 0; i < paralelos; i++) {
    // double z = -r + 2.0 * i / (paralelos - 1) * r;
    // double R = sqrt(r * r - z * z)
    double z = -1.0 + 2.0 * i / (paralelos - 1);
    double R = sqrt(1 - z * z);

    vector<Point> points;

    for (int j = 0; j < meridianos; j++) {
      double alpha = 2.0 * PI * j / (double)meridianos;
      Point point;
      point.x = R * cos(alpha);
      point.y = R * sin(alpha);
      point.z = z;
      points.push_back(point);
    }

    sphere.push_back(points);
  }

  return sphere;
}

void generate_ply_sphere(vector<vector<Point>> const &sphere) {
  ofstream file("sphere.ply");

  if (!file.is_open()) {
    cerr << "Error al abrir el archivo." << endl;
    return;
  }

  file << "ply\n";
  file << "format ascii 1.0\n";
  file << "element vertex " << sphere.size() * sphere[0].size() << "\n";
  file << "property float x\n";
  file << "property float y\n";
  file << "property float z\n";
  file << "property float s\n";
  file << "property float t\n";
  file << "element face " << (sphere.size() - 1) * sphere[0].size() << "\n";
  file << "property list uchar int vertex_index\n";
  file << "end_header\n";

  // for (const auto &points : sphere) {
  for (const auto &points : sphere) {
    for (const auto &point : points) {
      double s = (point.z + 1) / 2.0;
      double t = (atan2(point.y, point.x) + PI) / (2.0 * PI);
      file << point.x << " " << point.y << " " << point.z << " " << t << " "
           << s << "\n";
    }
  }

  for (int i = 0; i < sphere.size(); i++) {
    for (int j = 0; j < sphere[0].size(); j++) {
      int next_j = (j + 1) % sphere[0].size();

      file << "4 " << i * sphere[0].size() + j << " "
           << i * sphere[0].size() + next_j << " "
           << (i + 1) * sphere[0].size() + next_j << " "
           << (i + 1) * sphere[0].size() + j << "\n";
    }
  }

  file.close();
}

int main() {
  int n = 5;
  int m = 4;
  int r = 1;

  cout << "Generating sphere with " << n << " parallels and " << m
       << " meridians with radius " << r << endl;
  vector<vector<Point>> sphere = generate_sphere(n, m, r);
  generate_ply_sphere(sphere);
  return 0;
}
