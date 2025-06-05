#ifndef SOLUTION_H
#define SOLUTION_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
#define PI 3.14159265358979323846
#define PARALELOS 180
#define MERIDIANOS 360

struct Point {
  double x;
  double y;
  double z;
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
    file << "element face " << 2 * (PARALELOS - 1) * MERIDIANOS << "\n";
    file << "property list uchar int vertex_index\n";
    file << "end_header\n";
  }

  void printSphere(vector<vector<Point>> const &sphere) {
    for (const auto &row : sphere) {
      for (const auto &point : row) {
        file << point.x << " " << point.y << " " << point.z << "\n";
      }
    }

    for (int i = 0; i < PARALELOS - 1; ++i) {
      for (int j = 0; j < MERIDIANOS; ++j) {
        int next_j = (j + 1) % MERIDIANOS;

        file << "3 "
             << i * MERIDIANOS + j << " "
             << i * MERIDIANOS + next_j << " "
             << (i + 1) * MERIDIANOS + j << "\n";

        file << "3 "
             << i * MERIDIANOS + next_j << " "
             << (i + 1) * MERIDIANOS + next_j << " "
             << (i + 1) * MERIDIANOS + j << "\n";
      }
    }

    file.close();
  }
};

void sphere_with_triangular_faces(string const &full_path_output_file, double radius, double center_x, double center_y, double center_z) {
  vector<vector<Point>> sphere;

  for (int i = 0; i < PARALELOS; i++) {
    double z = -radius + 2.0 * i / (PARALELOS - 1) * radius;
    double R = sqrt(radius * radius - z * z);

    vector<Point> points;

    for (int j = 0; j < MERIDIANOS; j++) {
      double alpha = 2.0 * PI * j / (double)MERIDIANOS;
      Point point;
      point.x = center_x + R * cos(alpha);
      point.y = center_y + R * sin(alpha);
      point.z = center_z + z;
      points.push_back(point);
    }

    sphere.push_back(points);
  }

  cout << sphere.size() << " rows, " << sphere[0].size() << " columns" << endl;

  generatePLY ply(full_path_output_file);
  ply.printSphere(sphere);
}

#endif