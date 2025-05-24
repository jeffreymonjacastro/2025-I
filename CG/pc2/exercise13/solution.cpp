#include "douglas_peucker.h"

int main() {
  vector<vector<int>> points = {
      {0, 3},
      {1, 0},
      {2, 1},
      {4, 2},
      {5, 1},
      {6, 1},
      {8, 2},
  };

  double epsilon = 0.5;

  cout << "Puntos originales: ";
  for (const auto &point : points) {
    cout << "(" << point[0] << "," << point[1] << ") ";
  }
  cout << endl;

  auto result = douglas_peucker(points, epsilon);

  cout << "Puntos simplificados: ";
  for (const auto &point : result) {
    cout << "(" << point[0] << "," << point[1] << ") ";
  }
  cout << endl;
  return 0;
}
