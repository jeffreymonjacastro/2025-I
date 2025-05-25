#include "area_of_intersection.h"

int main() {
  vector<vector<double>> polygon1 = {
      {9.0, 5.0},
      {7.0, 8.0},
      {3.0, 8.0},
      {1.0, 5.0},
      {3.0, 2.0},
      {7.0, 2.0}};

  vector<vector<double>> polygon2 = {
      {10.0, 6.0},
      {8.0, 9.0},
      {5.0, 8.0},
      {5.0, 4.0},
      {8.0, 3.0}};

  double intersection_area = area_of_intersection(polygon1, polygon2);

  double area1 = area(polygon1);
  double area2 = area(polygon2);

  cout << "Area del poligono 1: " << area1 << endl;
  cout << "Area del poligono 2: " << area2 << endl;
  cout << "Area de interseccion: " << intersection_area << endl;

  return 0;
}