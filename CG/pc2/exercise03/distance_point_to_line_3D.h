#ifndef DISTANCE_POINT_TO_LINE_3D_H
#define DISTANCE_POINT_TO_LINE_3D_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
  double x, y, z;
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Vector {
  double x, y, z;
  Vector(double x, double y, double z) : x(x), y(y), z(z) {}

  Vector unit_vector() {
    double length = this->module();
    return Vector(x / length, y / length, z / length);
  }

  double module() { return sqrt(x * x + y * y + z * z); }
};

struct Line {
  Point l;
  Vector v;
  Line(Point l, Vector v) : l(l), v(v) {}
};

Vector cross_product(Vector A, Vector B) {
  double x = A.y * B.z - A.z * B.y;
  double y = A.z * B.x - A.x * B.z;
  double z = A.x * B.y - A.y * B.x;
  return Vector(x, y, z);
}

double distance_point_to_line_3D(double p1x, double p1y, double p1z, double dx,
                                 double dy, double dz, double px, double py,
                                 double pz) {
  Point p1(p1x, p1y, p1z);
  Vector v(dx, dy, dz);
  Point P(px, py, pz);
  Line L(p1, v);

  Vector unit_vector = L.v.unit_vector();
  Vector vector_point(P.x - L.l.x, P.y - L.l.y, P.z - L.l.z);

  Vector cross = cross_product(vector_point, unit_vector);
  return cross.module();
}

#endif