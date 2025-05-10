#ifndef INSIDE_TRIANGLE_H
#define INSIDE_TRIANGLE_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> T cross_product(T x1, T y1, T x2, T y2) {
  return x1 * y2 - x2 * y1;
}

template <typename T>
bool inside_triangle(vector<vector<T>> const &vertices, T px, T py) {
  T x1 = vertices[0][0], y1 = vertices[0][1];
  T x2 = vertices[1][0], y2 = vertices[1][1];
  T x3 = vertices[2][0], y3 = vertices[2][1];

  // Calculo el producto cruzado
  T cross1 = cross_product(x2 - x1, y2 - y1, px - x1, py - y1);
  T cross2 = cross_product(x3 - x2, y3 - y2, px - x2, py - y2);
  T cross3 = cross_product(x1 - x3, y1 - y3, px - x3, py - y3);

  return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0) ||
         (cross1 <= 0 && cross2 <= 0 && cross3 <= 0);
}

#endif