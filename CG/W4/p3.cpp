#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
  float x, y;
  Point(float x, float y) : x(x), y(y) {}
};

struct Vector {
  float x, y;
  Vector(float x, float y) : x(x), y(y) {}
};

struct Triangle {
  Point A, B, C;
  Triangle(Point A, Point B, Point C) : A(A), B(B), C(C) {}
};

float cross_product(Vector A, Vector B) { return A.x * B.y - B.x * A.y; }

int sgn(float num) { return (num > 0) - (num < 0); }

//* Determine if a point is inside a triangle in 2D
bool point_inside_triangle(Triangle t, Point p) {
  vector<int> inside(3);
  Vector AC(t.A.x - t.C.x, t.A.y - t.C.y);
  Vector AP(t.A.x - p.x, t.A.y - p.y);
  Vector CB(t.C.x - t.B.x, t.C.y - t.B.y);
  Vector CP(t.C.x - p.x, t.C.y - p.y);
  Vector BA(t.B.x - t.A.x, t.B.y - t.A.y);
  Vector BP(t.B.x - p.x, t.B.y - p.y);

  inside[0] = sgn(cross_product(AC, AP));
  inside[1] = sgn(cross_product(CB, CP));
  inside[2] = sgn(cross_product(BA, BP));

  if (inside[0] == inside[1] && inside[1] == inside[2]) {
    return true;
  } else if (inside[0] == 0 || inside[1] == 0 || inside[2] == 0) {
    return true;
  }
  return false;
}

int main() {
  Point A(1, 1);
  Point B(3, 4);
  Point C(5, 1);
  Triangle t(A, B, C);
  Point p(5, 2);

  cout << "Point inside triangle: " << point_inside_triangle(t, p) << endl;
  return 0;
}