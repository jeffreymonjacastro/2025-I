#ifndef INSIDE_POLYGON_H
#define INSIDE_POLYGON_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
bool inside_polygon(vector<vector<T>> const &vertices, T px, T py) {
  int n = vertices.size();
  bool inside = false;

  for (int i = 0, j = n - 1; i < n; j = i++) {
    T xi = vertices[i][0], yi = vertices[i][1];
    T xj = vertices[j][0], yj = vertices[j][1];

    if ((px == xi && py == yi) || (px == xj && py == yj)) {
      return true;
    }

    if (yi == yj && py == yi && (px >= min(xi, xj) && px <= max(xi, xj))) {
      return true;
    }

    if (yi == yj) {
      continue;
    }

    bool is_between_y_edge = (yi > py) != (yj > py);

    if (is_between_y_edge) {
      T x_intersect = (xj - xi) * (py - yi) / (yj - yi) + xi;

      if (px == x_intersect) {
        return true;
      }

      if (px < x_intersect) {
        inside = !inside;
      }
    }
  }

  return inside;
}

#endif