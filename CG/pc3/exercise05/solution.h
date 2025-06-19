#ifndef SOLUTION_H
#define SOLUTION_H
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#define PI 3.14159265358979323846

struct Point {
  double x;
  double y;
  double z;
  double s;
  double t;
  Point(double x = 0.0, double y = 0.0, double z = 0.0, double s = 0.0, double t = 0.0)
      : x(x), y(y), z(z), s(s), t(t) {}
};

struct Face {
  int v1, v2, v3;
  Face(int v1 = 0, int v2 = 0, int v3 = 0) : v1(v1), v2(v2), v3(v3) {}
};

class generatePLY {
private:
  ofstream file;
  int numVertices;
  int numFaces;

public:
  generatePLY(string const &filename, int vertices, int faces)
      : file(filename), numVertices(vertices), numFaces(faces) {
    if (!file.is_open()) {
      cerr << "Error opening file: " << filename << endl;
      exit(EXIT_FAILURE);
    }
    header();
  }

  void header() {
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << numVertices << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property float s\n";
    file << "property float t\n";
    file << "element face " << numFaces << "\n";
    file << "property list uchar int vertex_index\n";
    file << "end_header\n";
  }

  void writeVertex(const Point &point, double s, double t) {
    file << point.x << " " << point.y << " " << point.z << " "
         << s << " " << t << "\n";
  }

  void writeFace(int v1, int v2, int v3) {
    file << "3 " << v1 << " " << v2 << " " << v3 << "\n";
  }

  void close() {
    file.close();
  }
};

void sphere_with_texture(string full_path_input_ply, string full_path_texture,
                         Point center, string full_path_output_ply) {
  ifstream inputFile(full_path_input_ply);
  if (!inputFile.is_open()) {
    cerr << "Error opening input file: " << full_path_input_ply << endl;
    return;
  }

  string line;
  vector<Point> vertices;
  vector<Face> faces;
  int numVertices = 0, numFaces = 0;
  bool readingHeader = true;

  while (getline(inputFile, line)) {
    if (readingHeader) {
      if (line.find("element vertex") != string::npos) {
        stringstream ss(line);
        string temp1, temp2;
        ss >> temp1 >> temp2 >> numVertices;
      } else if (line.find("element face") != string::npos) {
        stringstream ss(line);
        string temp1, temp2;
        ss >> temp1 >> temp2 >> numFaces;
      } else if (line == "end_header") {
        readingHeader = false;
      }
    } else {
      if (vertices.size() < numVertices) {
        stringstream ss(line);
        Point p;
        ss >> p.x >> p.y >> p.z;

        float dx = p.x - center.x;
        float dy = p.y - center.y;
        float dz = p.z - center.z;

        float r = sqrt(dx * dx + dy * dy + dz * dz);

        if (r < 1e-10) {
          p.s = 0.0;
          p.t = 0.0;
        } else {
          float theta = atan2(sqrt(dx * dx + dy * dy), dz);
          float phi = atan2(dy, dx);

          p.s = (phi + PI) / (2 * PI);
          p.t = 1.0 - (theta / PI);
        }

        vertices.push_back(p);
      } else {
        stringstream ss(line);
        int n;
        Face c;
        ss >> n >> c.v1 >> c.v2 >> c.v3;
        faces.push_back(c);
      }
    }
  }
  inputFile.close();

  vector<Point> newVertices = vertices;
  vector<Face> newFaces;
  int nextIndex = vertices.size();

  for (auto &face : faces) {
    float s1 = vertices[face.v1].s;
    float s2 = vertices[face.v2].s;
    float s3 = vertices[face.v3].s;

    float maxDifference = max(max(abs(s1 - s2), abs(s2 - s3)), abs(s3 - s1));

    if (maxDifference > 0.5) {
      Face newFace = face;

      if (s1 < 0.5 && (s2 > 0.5 || s3 > 0.5)) {
        Point newPoint = vertices[face.v1];
        newPoint.s += 1.0;
        newVertices.push_back(newPoint);
        newFace.v1 = nextIndex++;
      }

      if (s2 < 0.5 && (s1 > 0.5 || s3 > 0.5)) {
        Point newPoint = vertices[face.v2];
        newPoint.s += 1.0;
        newVertices.push_back(newPoint);
        newFace.v2 = nextIndex++;
      }

      if (s3 < 0.5 && (s1 > 0.5 || s2 > 0.5)) {
        Point newPoint = vertices[face.v3];
        newPoint.s += 1.0;
        newVertices.push_back(newPoint);
        newFace.v3 = nextIndex++;
      }

      newFaces.push_back(newFace);
    } else {
      newFaces.push_back(face);
    }
  }

  generatePLY plyWriter(full_path_output_ply, newVertices.size(), newFaces.size());

  for (auto &p : newVertices) {
    plyWriter.writeVertex(p, p.s, p.t);
  }

  for (auto &c : newFaces) {
    plyWriter.writeFace(c.v1, c.v2, c.v3);
  }

  plyWriter.close();
}

#endif