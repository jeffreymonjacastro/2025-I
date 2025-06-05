#ifndef SOLUTION_H
#define SOLUTION_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
#define PI 3.14159265358979323846
#define PARALELOS 180
#define MERIDIANOS 360
#define radius 1.0

struct Point {
  double x;
  double y;
  double z;

  Point(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
};

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
    file << "element vertex " << PARALELOS * MERIDIANOS << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property float s\n";
    file << "property float t\n";
    file << "element face " << (PARALELOS - 1) * MERIDIANOS << "\n";
    file << "property list uchar int vertex_index\n";
    file << "end_header\n";
  }

  void printSphere(vector<vector<Point>> const &sphere) {
    for (const auto &row : sphere) {
      for (const auto &point : row) {
        double s = (atan2(point.y, point.x) + PI) / (2 * PI);
        double t = (point.z + radius) / (2 * radius);
        file << point.x << " " << point.y << " " << point.z << " " << s << " " << t << "\n";
      }
    }

    for (int i = 0; i < PARALELOS - 1; ++i) {
      for (int j = 0; j < MERIDIANOS; ++j) {
        int next_j = (j + 1) % MERIDIANOS;

        file << "4 "
             << i * MERIDIANOS + j << " "
             << i * MERIDIANOS + next_j << " "
             << (i + 1) * MERIDIANOS + next_j << " "
             << (i + 1) * MERIDIANOS + j << "\n";
      }
    }

    file.close();
  }
};

void sphere_with_texture(string full_path_input_ply, string full_path_texture, Point center, string full_path_output_ply) {
  fstream sphere(full_path_input_ply, ios::in);
  if (!sphere.is_open()) {
    cerr << "Error opening file: " << full_path_input_ply << endl;
    return;
  }

  vector<vector<Point>> sphere;

  string line;
  while (getline(sphere, line)) {
    istringstream iss(line);
    Point point;
    if (iss >> point.x >> point.y >> point.z) {
      sphere.push_back(point);
    }
  }

  generatePLY ply(full_path_output_ply);
  ply.printSphere(sphere);
}

#endif