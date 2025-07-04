#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
#define PI 3.14159265358979323846
#define PARALELOS 45 
#define MERIDIANOS 90

struct Point {
  double x;
  double y;
  double z;
  int r;  
  int g;  
  int b;  
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
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "end_header\n";
  }

  void printSphere(vector<vector<Point>> const &sphere) {
    for (const auto &row : sphere) {
      for (const auto &point : row) {
        file << point.x << " " << point.y << " " << point.z << " "
             << point.r << " " << point.g << " " << point.b << "\n";
      }
    }

    file.close();
  }
};

void HSVtoRGB(double h, double s, double v, int &r, int &g, int &b) {
  double c = v * s;
  double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
  double m = v - c;
  
  double r_prime, g_prime, b_prime;
  
  if (h >= 0 && h < 60) {
    r_prime = c; g_prime = x; b_prime = 0;
  } else if (h >= 60 && h < 120) {
    r_prime = x; g_prime = c; b_prime = 0;
  } else if (h >= 120 && h < 180) {
    r_prime = 0; g_prime = c; b_prime = x;
  } else if (h >= 180 && h < 240) {
    r_prime = 0; g_prime = x; b_prime = c;
  } else if (h >= 240 && h < 300) {
    r_prime = x; g_prime = 0; b_prime = c;
  } else {
    r_prime = c; g_prime = 0; b_prime = x;
  }
  
  r = (int)((r_prime + m) * 255);
  g = (int)((g_prime + m) * 255);
  b = (int)((b_prime + m) * 255);
}

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
      
      double height_factor = (z + radius) / (2.0 * radius);
      
      double angle_factor = (double)j / (double)MERIDIANOS;
      
      double hue = 300.0 * (1.0 - height_factor); 
      
      double saturation = 1.0;
      
      double meridian_variation = sin(angle_factor * PI * 3.0); 
      double value = 0.8 + 0.2 * meridian_variation; 
      
      HSVtoRGB(hue, saturation, value, point.r, point.g, point.b);
      
      point.r = max(0, min(255, point.r));
      point.g = max(0, min(255, point.g));
      point.b = max(0, min(255, point.b));
      
      points.push_back(point);
    }

    sphere.push_back(points);
  }

  cout << sphere.size() << " rows, " << sphere[0].size() << " columns" << endl;

  generatePLY ply(full_path_output_file);
  ply.printSphere(sphere);
}

int main() {
  double cx = 0.0;
  double cy = 0.0;
  double cz = 0.0;
  double R = 1.0;
  sphere_with_triangular_faces("sphere.ply", R, cx, cy, cz);
  cout << "Colored sphere generated" << endl;

  return 0;
}