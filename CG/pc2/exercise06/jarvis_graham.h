#ifndef JARVIS_H
#define JARVIS_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

template <typename T>
T cross_product(const vector<T> &p0, const vector<T> &p1, const vector<T> &p2) {
  return (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p2[0] - p0[0]) * (p1[1] - p0[1]);
}

template <typename T>
double euclidean_distance(const vector<T> &p0, const vector<T> &p1) {
  return sqrt(pow(p1[0] - p0[0], 2) + pow(p1[1] - p0[1], 2));
}

template <typename T> void print_hull(const vector<vector<T>> &hull) {
  for (const auto &point : hull) {
    cout << point[0] << " " << point[1] << endl;
  }
}

template <typename T> auto make_comparator(const vector<T> &base_point) {
  return [base_point](const vector<T> &a, const vector<T> &b) -> bool {
    T cross = cross_product(base_point, a, b);
    if (cross == 0) {
      double dist_a = euclidean_distance(base_point, a);
      double dist_b = euclidean_distance(base_point, b);
      return dist_a < dist_b;
    }
    return cross > 0;
  };
}

template <typename T>
bool inside_quadrilateral(const vector<T> &p, const vector<T> &a,
                          const vector<T> &b, const vector<T> &c,
                          const vector<T> &d) {
  T cross1 = cross_product(a, b, p);
  T cross2 = cross_product(b, c, p);
  T cross3 = cross_product(c, d, p);
  T cross4 = cross_product(d, a, p);

  return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0) ||
         (cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0);
}

template <typename T>
vector<vector<T>> remove_colineal(vector<vector<T>> const &vertices) {
  int n = vertices.size();
  if (n < 3) {
    return vertices;
  }

  int base_idx = 0;
  for (int i = 1; i < n; i++) {
    if (vertices[i][1] < vertices[base_idx][1] ||
        (vertices[i][1] == vertices[base_idx][1] &&
         vertices[i][0] < vertices[base_idx][0])) {
      base_idx = i;
    }
  }

  vector<vector<T>> v = vertices;

  swap(v[0], v[base_idx]);
  vector<T> base_point = v[0];

  sort(v.begin() + 1, v.end(), make_comparator(base_point));

  vector<vector<T>> filtered;
  filtered.push_back(v[0]);

  for (int i = 1; i < n; i++) {
    int j = i;
    while (j < n - 1 && cross_product(base_point, v[i], v[j + 1]) == 0) {
      j++;
    }

    if (j == i) {
      filtered.push_back(v[i]);
    } else {
      filtered.push_back(v[j]);
      i = j;
    }
  }

  return filtered;
}

template <typename T>
vector<vector<T>> interior_points_removal(vector<vector<T>> const &vertices) {
  int n = vertices.size();
  if (n <= 4) {
    return vertices;
  }

  vector<T> leftmost = vertices[0];
  vector<T> rightmost = vertices[0];
  vector<T> topmost = vertices[0];
  vector<T> bottommost = vertices[0];

  for (int i = 1; i < n; i++) {
    if (vertices[i][0] < leftmost[0] ||
        (vertices[i][0] == leftmost[0] && vertices[i][1] < leftmost[1])) {
      leftmost = vertices[i];
    }

    if (vertices[i][0] > rightmost[0] ||
        (vertices[i][0] == rightmost[0] && vertices[i][1] > rightmost[1])) {
      rightmost = vertices[i];
    }

    if (vertices[i][1] > topmost[1] ||
        (vertices[i][1] == topmost[1] && vertices[i][0] < topmost[0])) {
      topmost = vertices[i];
    }

    if (vertices[i][1] < bottommost[1] ||
        (vertices[i][1] == bottommost[1] && vertices[i][0] > bottommost[0])) {
      bottommost = vertices[i];
    }
  }

  vector<vector<T>> extreme_points;
  extreme_points.push_back(leftmost);

  if (rightmost != leftmost)
    extreme_points.push_back(rightmost);
  if (topmost != leftmost && topmost != rightmost)
    extreme_points.push_back(topmost);
  if (bottommost != leftmost && bottommost != rightmost &&
      bottommost != topmost) {
    extreme_points.push_back(bottommost);
  }

  if (extreme_points.size() < 3) {
    return remove_colineal(vertices);
  }

  vector<vector<T>> result;

  for (int i = 0; i < n; i++) {
    bool is_extreme = (vertices[i] == leftmost || vertices[i] == rightmost ||
                       vertices[i] == topmost || vertices[i] == bottommost);

    if (is_extreme) {
      result.push_back(vertices[i]);
      continue;
    }

    bool is_inside = false;

    is_inside = inside_quadrilateral(vertices[i], leftmost, bottommost,
                                     rightmost, topmost);

    if (!is_inside) {
      result.push_back(vertices[i]);
    }
  }

  return result;
}

// JARVIS
template <typename T>
vector<vector<T>> jarvis(vector<vector<T>> const &vertices,
                         bool withInteriorPointsRemoval) {
  int n = vertices.size();
  if (n < 3) {
    return vertices;
  }

  vector<vector<T>> v = vertices;

  if (withInteriorPointsRemoval) {
    v = interior_points_removal(vertices);
  }

  n = v.size();
  if (n < 3) {
    return v;
  }

  int min_point = 0;

  for (int i = 1; i < n; i++) {
    if (v[i][1] < v[min_point][1] ||
        (v[i][1] == v[min_point][1] && v[i][0] < v[min_point][0])) {
      min_point = i;
    }
  }

  vector<vector<T>> hull;
  int curr = min_point;
  int next = 0;

  do {
    hull.push_back(v[curr]);
    next = (curr + 1) % n;

    for (int i = 0; i < n; i++) {
      T cross = cross_product(v[curr], v[next], v[i]);

      if (cross < 0) {
        next = i;
      } else if (cross == 0) {
        double dist1 = euclidean_distance(v[curr], v[next]);
        double dist2 = euclidean_distance(v[curr], v[i]);

        if (dist2 > dist1) {
          next = i;
        }
      }
    }

    curr = next;

  } while (curr != min_point);

  // print_hull(hull);

  return hull;
}

// GRAHAM
template <typename T>
vector<vector<T>> graham(vector<vector<T>> const &vertices,
                         bool withInteriorPointsRemoval) {
  int n = vertices.size();

  if (n < 3) {
    return vertices;
  }

  vector<vector<T>> v = vertices;

  if (withInteriorPointsRemoval) {
    v = interior_points_removal(vertices);
  }

  n = v.size();
  if (n < 3) {
    return v;
  }

  int min_idx = 0;

  for (int i = 1; i < n; i++) {
    if (v[i][1] < v[min_idx][1] ||
        (v[i][1] == v[min_idx][1] && v[i][0] < v[min_idx][0])) {
      min_idx = i;
    }
  }

  swap(v[0], v[min_idx]);
  vector<T> min_point = v[0];

  sort(v.begin() + 1, v.end(), make_comparator(min_point));

  stack<vector<T>> hull;
  hull.push(v[0]);
  hull.push(v[1]);
  hull.push(v[2]);

  for (int i = 3; i < n; i++) {
    while (hull.size() >= 2) {
      vector<T> top = hull.top();
      hull.pop();
      vector<T> next_to_top = hull.top();

      T cross = cross_product(next_to_top, top, v[i]);

      if (cross > 0) {
        hull.push(top);
        break;
      }
    }
    hull.push(v[i]);
  }

  vector<vector<T>> result;
  while (!hull.empty()) {
    result.push_back(hull.top());
    hull.pop();
  }

  reverse(result.begin(), result.end());

  // print_hull(result);

  return result;
};

#endif