#ifndef INSIDE_POLYGON_H
#define INSIDE_POLYGON_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> struct Point {
  T x, y;
  Point(T x, T y) : x(x), y(y) {}
};

template <typename T> struct Vector {
  T x, y;
  Vector(T x, T y) : x(x), y(y) {}

  Vector unit_vector() {
    double length = this->module();
    return Vector(x / length, y / length, z / length);
  }

  double module() { return sqrt(x * x + y * y); }
};

template <typename T> struct Line {
  Point p;
  Vector v;
  Line(Point p, Vector v) : p(p), v(v) {}
};

template <typename T> T cross_product(T x1, T y1, T x2, T y2) {
  return x1 * y2 - x2 * y1;
}

template <typename T>
bool inside_polygon(vector<vector<T>> const &vertices, T px, T py) {
  cout << "Hello world" << endl;
}

#endif