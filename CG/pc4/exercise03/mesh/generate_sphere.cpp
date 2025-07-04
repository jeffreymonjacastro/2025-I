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

struct Face {
  int vertex1;
  int vertex2;
  int vertex3;
  int r;
  int g;
  int b;
};

class generatePLY {
private:
  ofstream file;
  int num_vertices;
  int num_faces;

public:
  generatePLY(string const &s, int vertices, int faces) : file(s), num_vertices(vertices), num_faces(faces) {
    if (!file.is_open()) {
      cerr << "Error opening file: " << s << endl;
      exit(EXIT_FAILURE);
    }

    header();
  }

  void header() {
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << num_vertices << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "element face " << num_faces << "\n";
    file << "property list uchar int vertex_indices\n";
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "end_header\n";
  }

  void printSphere(vector<vector<Point>> const &sphere, vector<Face> const &faces) {
    // Escribir vértices
    for (const auto &row : sphere) {
      for (const auto &point : row) {
        file << point.x << " " << point.y << " " << point.z << " "
             << point.r << " " << point.g << " " << point.b << "\n";
      }
    }
    
    // Escribir caras
    for (const auto &face : faces) {
      file << "3 " << face.vertex1 << " " << face.vertex2 << " " << face.vertex3 << " "
           << face.r << " " << face.g << " " << face.b << "\n";
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

vector<Face> generateFaces(const vector<vector<Point>>& sphere) {
  vector<Face> faces;
  
  for (int i = 0; i < PARALELOS - 1; i++) {
    for (int j = 0; j < MERIDIANOS; j++) {
      int next_j = (j + 1) % MERIDIANOS;
      
      // Índices de los vértices
      int v1 = i * MERIDIANOS + j;
      int v2 = i * MERIDIANOS + next_j;
      int v3 = (i + 1) * MERIDIANOS + j;
      int v4 = (i + 1) * MERIDIANOS + next_j;
      
      // Primer triángulo
      Face face1;
      face1.vertex1 = v1;
      face1.vertex2 = v2;
      face1.vertex3 = v3;
      
      // Color promedio de los vértices del triángulo
      face1.r = (sphere[i][j].r + sphere[i][next_j].r + sphere[i + 1][j].r) / 3;
      face1.g = (sphere[i][j].g + sphere[i][next_j].g + sphere[i + 1][j].g) / 3;
      face1.b = (sphere[i][j].b + sphere[i][next_j].b + sphere[i + 1][j].b) / 3;
      
      faces.push_back(face1);
      
      // Segundo triángulo
      Face face2;
      face2.vertex1 = v2;
      face2.vertex2 = v4;
      face2.vertex3 = v3;
      
      // Color promedio de los vértices del triángulo
      face2.r = (sphere[i][next_j].r + sphere[i + 1][next_j].r + sphere[i + 1][j].r) / 3;
      face2.g = (sphere[i][next_j].g + sphere[i + 1][next_j].g + sphere[i + 1][j].g) / 3;
      face2.b = (sphere[i][next_j].b + sphere[i + 1][next_j].b + sphere[i + 1][j].b) / 3;
      
      faces.push_back(face2);
    }
  }
  
  return faces;
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

  // Generar caras
  vector<Face> faces = generateFaces(sphere);
  int num_faces = faces.size();
  cout << "Generated " << num_faces << " faces" << endl;
  
  // Calcular número de vértices y caras
  int num_vertices = PARALELOS * MERIDIANOS;

  generatePLY ply(full_path_output_file, num_vertices, num_faces);
  ply.printSphere(sphere, faces);
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