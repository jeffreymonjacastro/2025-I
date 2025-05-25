#ifndef AREA_OF_INTERSECTION_H
#define AREA_OF_INTERSECTION_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#define ll long long int
#define ld long double

using namespace std;

template <typename T = int>
struct Point {
  T x, y;
  Point(T x, T y) : x(x), y(y) {}
};

template <typename T = ll>
struct Segment {
  Point<T> left, right;
  Segment(Point<T> left, Point<T> right) : left(left), right(right) {
    if (left.x > right.x) {
      this->left = right;
      this->right = left;
    } else if (left.x == right.x && left.y > right.y) {
      this->left = right;
      this->right = left;
    } else {
      this->left = left;
      this->right = right;
    }
  }
};

ll cross_product(Point<int> p1, Point<int> p2, Point<int> p3) {
  ll x1 = (ll)p2.x - (ll)p1.x;
  ll y1 = (ll)p2.y - (ll)p1.y;
  ll x2 = (ll)p3.x - (ll)p1.x;
  ll y2 = (ll)p3.y - (ll)p1.y;
  return x1 * y2 - x2 * y1;
}

ld cross_product(Point<double> p1, Point<double> p2, Point<double> p3) {
  ld x1 = (ld)p2.x - (ld)p1.x;
  ld y1 = (ld)p2.y - (ld)p1.y;
  ld x2 = (ld)p3.x - (ld)p1.x;
  ld y2 = (ld)p3.y - (ld)p1.y;
  return x1 * y2 - x2 * y1;
}

template <typename T>
int sgn(T num) {
  return (num > 0) - (num < 0);
}

template <typename T>
bool intersection(T xa, T ya, T xb, T yb, T xc, T yc, T xd, T yd) {
  Point<T> p1(xa, ya), p2(xb, yb), p3(xc, yc), p4(xd, yd);
  Segment<T> s1(p1, p2), s2(p3, p4);

  auto P1P2_cross_P1P3 = cross_product(p1, p2, p3);
  int P1P2_cross_P1P3_sign = sgn(P1P2_cross_P1P3);
  auto P1P2_cross_P1P4 = cross_product(p1, p2, p4);
  int P1P2_cross_P1P4_sign = sgn(P1P2_cross_P1P4);

  if (P1P2_cross_P1P3 == 0 && P1P2_cross_P1P4 == 0) {
    T min_x1 = min(s1.left.x, s1.right.x);
    T max_x1 = max(s1.left.x, s1.right.x);
    T min_y1 = min(s1.left.y, s1.right.y);
    T max_y1 = max(s1.left.y, s1.right.y);
    T min_x2 = min(s2.left.x, s2.right.x);
    T max_x2 = max(s2.left.x, s2.right.x);
    T min_y2 = min(s2.left.y, s2.right.y);
    T max_y2 = max(s2.left.y, s2.right.y);
    if (min_x1 <= max_x2 && max_x1 >= min_x2 && min_y1 <= max_y2 && max_y1 >= min_y2) {
      return true;
    }
    return false;
  }

  if (P1P2_cross_P1P3_sign == P1P2_cross_P1P4_sign) {
    return false;
  }

  auto P3P4_cross_P3P1 = cross_product(p3, p4, p1);
  int P3P4_cross_P3P1_sign = sgn(P3P4_cross_P3P1);
  auto P3P4_cross_P3P2 = cross_product(p3, p4, p2);
  int P3P4_cross_P3P2_sign = sgn(P3P4_cross_P3P2);
  if (P3P4_cross_P3P1_sign == P3P4_cross_P3P2_sign) {
    return false;
  }
  return true;
}

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

template <typename T>
double area(vector<vector<T>> const &vertices) {
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

template <typename T>
vector<double> line_intersection(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) {
  double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (abs(denom) < 1e-10) {
    return {};
  }

  double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;

  double x = x1 + t * (x2 - x1);
  double y = y1 + t * (y2 - y1);

  return {(T)x, (T)y};
}

template <typename T>
bool is_inside_edge(T px, T py, T x1, T y1, T x2, T y2) {
  Point<T> p1(x1, y1), p2(x2, y2), p(px, py);
  auto cross = cross_product(p1, p2, p);
  return cross >= 0;
}

template <typename T>
double area_of_intersection(vector<vector<T>> const &vertices1, vector<vector<T>> const &vertices2) {
  if (vertices1.size() < 3 || vertices2.size() < 3) {
    return 0.0;
  }

  vector<vector<T>> result = vertices1;

  int n2 = vertices2.size();

  for (int i = 0; i < n2; ++i) {
    if (result.empty())
      break;

    int j = (i + 1) % n2;
    T edge_x1 = vertices2[i][0], edge_y1 = vertices2[i][1];
    T edge_x2 = vertices2[j][0], edge_y2 = vertices2[j][1];

    vector<vector<T>> new_result;

    if (!result.empty()) {
      int n = result.size();

      for (int k = 0; k < n; ++k) {
        int l = (k + 1) % n;
        T curr_x = result[k][0], curr_y = result[k][1];
        T next_x = result[l][0], next_y = result[l][1];

        bool curr_inside = is_inside_edge(curr_x, curr_y, edge_x1, edge_y1, edge_x2, edge_y2);
        bool next_inside = is_inside_edge(next_x, next_y, edge_x1, edge_y1, edge_x2, edge_y2);

        if (next_inside) {
          if (!curr_inside) {
            auto intersect = line_intersection(curr_x, curr_y, next_x, next_y,
                                               edge_x1, edge_y1, edge_x2, edge_y2);
            if (!intersect.empty()) {
              new_result.push_back(intersect);
            }
          }
          new_result.push_back({next_x, next_y});
        } else if (curr_inside) {
          auto intersect = line_intersection(curr_x, curr_y, next_x, next_y,
                                             edge_x1, edge_y1, edge_x2, edge_y2);
          if (!intersect.empty()) {
            new_result.push_back(intersect);
          }
        }
      }
    }

    result = new_result;
  }
  if (result.empty()) {
    bool all_inside = true;
    for (const auto &vertex : vertices1) {
      if (!inside_polygon(vertices2, vertex[0], vertex[1])) {
        all_inside = false;
        break;
      }
    }
    if (all_inside) {
      return area(vertices1);
    }

    all_inside = true;
    for (const auto &vertex : vertices2) {
      if (!inside_polygon(vertices1, vertex[0], vertex[1])) {
        all_inside = false;
        break;
      }
    }
    if (all_inside) {
      return area(vertices2);
    }

    return 0.0;
  }

  return area(result);
}

#endif