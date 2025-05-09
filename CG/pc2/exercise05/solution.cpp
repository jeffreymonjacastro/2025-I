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
bool inside_polygon(vector<vector<T>> const &vertices, T px, T py) {}

template <typename T> bool is_convex(vector<vector<T>> const &P) {
  int n = P.size();
  bool onLeft = true;
  bool onRight = true;

  // Recorro cada punto del poligono
  for (int i = 0; i < n; i++) {
    // Mapeo el siguiente y subsiguiente punto
    int j = (i + 1) % n;
    int k = (i + 2) % n;

    // Calculo el vector IJ y el vector JK
    T x1 = P[j][0] - P[i][0];
    T y1 = P[j][1] - P[i][1];
    T x2 = P[k][0] - P[j][0];
    T y2 = P[k][1] - P[j][1];

    // Calculo el producto cruzado
    T cross = cross_product(x1, y1, x2, y2);
    cout << "cross: " << cross << endl;

    if (cross < 0) {
      onLeft = false;
    } else if (cross > 0) {
      onRight = false;
    }
  }

  // Si todos los puntos están a la izquierda o a la derecha, pero no en ambos,
  // es convex
  if (onLeft ^ onRight)
    return true;
  return false;
}

int main() {
  int n = 4;
  vector<vector<int>> vertices(n, vector<int>(2));
  vertices[0] = {0, 0};
  vertices[1] = {1, 0};
  vertices[2] = {2, 0};
  vertices[3] = {1, 1};

  cout << boolalpha;

  return 0;
}