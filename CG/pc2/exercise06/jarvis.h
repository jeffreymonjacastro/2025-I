#ifndef JARVIS_H
#define JARVIS_H

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
vector<vector<T>> interior_points_removal(vector<vector<T>> const &vertices);

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

template <typename T>
vector<vector<T>> jarvis(vector<vector<T>> const &vertices,
                         bool withInteriorPointsRemoval) {
  int n = vertices.size();
  if (n < 3) {
    return vertices;
  }

  int min_point = 0;

  for (int i = 1; i < n; i++) {
    if (vertices[i][1] < vertices[min_point][1] ||
        (vertices[i][1] == vertices[min_point][1] &&
         vertices[i][0] < vertices[min_point][0])) {
      min_point = i;
    }
  }

  vector<vector<T>> hull;
  int curr = min_point;
  int next = 0;

  do {
    hull.push_back(vertices[curr]);
    next = (curr + 1) % n;

    for (int i = 0; i < n; i++) {
      T cross = cross_product(vertices[curr], vertices[next], vertices[i]);

      if (cross < 0) {
        next = i;
      } else if (cross == 0) {
        double dist1 = euclidean_distance(vertices[curr], vertices[next]);
        double dist2 = euclidean_distance(vertices[curr], vertices[i]);

        if (dist2 > dist1) {
          next = i;
        }
      }
    }

    curr = next;

  } while (curr != min_point);

  print_hull(hull);

  return hull;
}

#endif