#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <cmath>
#include <iostream>

#define ll long long int
#define ld long double

template <typename T = int> struct Point {
  T x, y;
  Point(T x, T y) : x(x), y(y) {}
};

// template <typename T> std::ostream &operator<<(std::ostream &os, const
// Point<T> &p) {
//   os << "(" << p.x << ", " << p.y << ")";
//   return os;
// }

template <typename T = ll> struct Segment {
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

inline ll cross_product(Point<int> p1, Point<int> p2, Point<int> p3) {
  ll x1 = (ll)p2.x - (ll)p1.x;
  ll y1 = (ll)p2.y - (ll)p1.y;
  ll x2 = (ll)p3.x - (ll)p1.x;
  ll y2 = (ll)p3.y - (ll)p1.y;
  return x1 * y2 - x2 * y1;
}

inline ld cross_product(Point<double> p1, Point<double> p2, Point<double> p3) {
  ld x1 = (ld)p2.x - (ld)p1.x;
  ld y1 = (ld)p2.y - (ld)p1.y;
  ld x2 = (ld)p3.x - (ld)p1.x;
  ld y2 = (ld)p3.y - (ld)p1.y;
  return x1 * y2 - x2 * y1;
}

template <typename T> int sgn(T num) { return (num > 0) - (num < 0); }

template <typename T>
bool intersection(T xa, T ya, T xb, T yb, T xc, T yc, T xd, T yd) {
  Point<T> p1(xa, ya), p2(xb, yb), p3(xc, yc), p4(xd, yd);
  Segment<T> s1(p1, p2), s2(p3, p4);

  // Aqui calculo si el vector P1P2 está a la izquierda o a la derecha de los
  // demás
  auto P1P2_cross_P1P3 = cross_product(p1, p2, p3);
  int P1P2_cross_P1P3_sign = sgn(P1P2_cross_P1P3);

  auto P1P2_cross_P1P4 = cross_product(p1, p2, p4);
  int P1P2_cross_P1P4_sign = sgn(P1P2_cross_P1P4);

  // Aqui verifico si ambos segmentos son colineales
  if (P1P2_cross_P1P3 == 0 && P1P2_cross_P1P4 == 0) {
    T min_x1 = std::min(s1.left.x, s1.right.x);
    T max_x1 = std::max(s1.left.x, s1.right.x);
    T min_y1 = std::min(s1.left.y, s1.right.y);
    T max_y1 = std::max(s1.left.y, s1.right.y);
    T min_x2 = std::min(s2.left.x, s2.right.x);
    T max_x2 = std::max(s2.left.x, s2.right.x);
    T min_y2 = std::min(s2.left.y, s2.right.y);
    T max_y2 = std::max(s2.left.y, s2.right.y);

    if (min_x1 <= max_x2 && max_x1 >= min_x2 && min_y1 <= max_y2 &&
        max_y1 >= min_y2) {
      return true;
    }
    return false;
  }

  // Aqui verifico si los segmentos se cruzan
  if (P1P2_cross_P1P3_sign == P1P2_cross_P1P4_sign) {
    return false;
  }

  // Compruebo con otros vectores de p3 a todos los demás puntos
  auto P3P4_cross_P3P1 = cross_product(p3, p4, p1);
  int P3P4_cross_P3P1_sign = sgn(P3P4_cross_P3P1);

  auto P3P4_cross_P3P2 = cross_product(p3, p4, p2);
  int P3P4_cross_P3P2_sign = sgn(P3P4_cross_P3P2);

  if (P3P4_cross_P3P1_sign == P3P4_cross_P3P2_sign) {
    return false;
  }

  return true;
}

#endif
