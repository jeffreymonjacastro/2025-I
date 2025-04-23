#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
  float x, y, z;
  Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector {
  float x, y, z;
  Vector(float x, float y, float z) : x(x), y(y), z(z) {}

  Vector unit_vector() {
    float length = this->module();
    return Vector(x / length, y / length, z / length);
  }

  float module() { return sqrt(x * x + y * y + z * z); }
};

struct Line {
  Point l;
  Vector v;
  Line(Point l, Vector v) : l(l), v(v) {}
};

Vector cross_product(Vector A, Vector B) {
  float x = A.y * B.z - A.z * B.y;
  float y = A.z * B.x - A.x * B.z;
  float z = A.x * B.y - A.y * B.x;
  return Vector(x, y, z);
}

//* Distance of point to a Line in 3D
float distance_point_line(Line L, Point P) {
  Vector unit_vector = L.v.unit_vector();
  Vector vector_point(P.x - L.l.x, P.y - L.l.y, P.z - L.l.z);

  Vector cross = cross_product(vector_point, unit_vector);
  return cross.module();
}

int main() {
  Point l(0, 0, 0);
  Vector v(1, 1, 1);
  Line L(l, v);
  Point p(2, 2, 2);
  cout << "Distance: " << distance_point_line(L, p) << endl;
  return 0;
}