#ifndef AREA_H
#define AREA_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> double area(vector<vector<T>> const &vertices) {
  double area = 0.0;
  int n = vertices.size();
  if (n < 3) {
    cerr << "Not enough vertices to form a polygon." << endl;
    return area;
  }

  for (int i = 0; i < n; ++i) {
    int j = (i + 1) % n;
    area += vertices[i][0] * vertices[j][1];
    area -= vertices[j][0] * vertices[i][1];
  }
  area = abs(area) / 2.0;
  return area;
}

#endif