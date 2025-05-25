#ifndef CLOSEST_POINTS_H
#define CLOSEST_POINTS_H

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

template <typename T>
T distance_squared(const vector<vector<T>> &points, int i, int j) {
  T dx = points[i][0] - points[j][0];
  T dy = points[i][1] - points[j][1];
  return dx * dx + dy * dy;
}

template <typename T>
pair<T, vector<int>> brute_force_closest(const vector<vector<T>> &points, const vector<int> &indices) {
  T min_dist = numeric_limits<T>::max();
  vector<int> result = {indices[0], indices[1]};

  for (int i = 0; i < indices.size(); i++) {
    for (int j = i + 1; j < indices.size(); j++) {
      T dist = distance_squared(points, indices[i], indices[j]);
      if (dist < min_dist) {
        min_dist = dist;
        result = {indices[i], indices[j]};
      }
    }
  }

  // Ordenar los índices de menor a mayor
  if (result[0] > result[1]) {
    swap(result[0], result[1]);
  }

  return {min_dist, result};
}

template <typename T>
pair<T, vector<int>> closest_pair_rec(const vector<vector<T>> &points,
                                      vector<int> &px_subset,
                                      vector<int> &py_subset) {
  int n = px_subset.size();

  // Caso base: usar fuerza bruta para 3 o menos puntos
  if (n <= 3) {
    return brute_force_closest(points, px_subset);
  }

  // Dividir en dos mitades
  int mid = n / 2;
  T midpoint = points[px_subset[mid]][0];

  vector<int> pyl(mid), pyr(n - mid);
  vector<int> pxl(px_subset.begin(), px_subset.begin() + mid);
  vector<int> pxr(px_subset.begin() + mid, px_subset.end());

  // Dividir py en mitades izquierda y derecha
  int left_idx = 0, right_idx = 0;
  for (int i = 0; i < n; i++) {
    if (points[py_subset[i]][0] < midpoint ||
        (points[py_subset[i]][0] == midpoint && left_idx < mid)) {
      pyl[left_idx++] = py_subset[i];
    } else {
      pyr[right_idx++] = py_subset[i];
    }
  }

  // Resolver recursivamente ambas mitades
  auto left_result = closest_pair_rec(points, pxl, pyl);
  auto right_result = closest_pair_rec(points, pxr, pyr);

  // Encontrar el mínimo de las dos mitades
  T delta_squared;
  vector<int> best_pair;
  if (left_result.first < right_result.first) {
    delta_squared = left_result.first;
    best_pair = left_result.second;
  } else {
    delta_squared = right_result.first;
    best_pair = right_result.second;
  }

  T delta = sqrt(delta_squared);

  // Crear array de puntos dentro de la franja
  vector<int> strip;
  for (int i = 0; i < n; i++) {
    if (abs(points[py_subset[i]][0] - midpoint) < delta) {
      strip.push_back(py_subset[i]);
    }
  }

  // Verificar puntos en la franja
  for (int i = 0; i < strip.size(); i++) {
    for (int j = i + 1; j < strip.size() &&
                        (points[strip[j]][1] - points[strip[i]][1]) < delta;
         j++) {
      T dist = distance_squared(points, strip[i], strip[j]);
      if (dist < delta_squared) {
        delta_squared = dist;
        best_pair = {strip[i], strip[j]};
        // Ordenar los índices
        if (best_pair[0] > best_pair[1]) {
          swap(best_pair[0], best_pair[1]);
        }
        delta = sqrt(delta_squared);
      }
    }
  }

  return {delta_squared, best_pair};
}

template <typename T>
vector<int> closest_points(vector<vector<T>> const &points) {
  int n = points.size();

  // Crear vectores de índices ordenados por x e y
  vector<int> px(n), py(n);
  for (int i = 0; i < n; i++) {
    px[i] = py[i] = i;
  }

  // Ordenar por coordenada x
  sort(px.begin(), px.end(), [&](int i, int j) {
    return points[i][0] < points[j][0];
  });

  // Ordenar por coordenada y
  sort(py.begin(), py.end(), [&](int i, int j) {
    return points[i][1] < points[j][1];
  });

  auto result = closest_pair_rec(points, px, py);
  return result.second;
}

#endif // CLOSEST_POINTS_H