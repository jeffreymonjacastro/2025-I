#include "closest_points.h"

int main() {
  cout << "=== Caso de Prueba 1: Puntos basicos ===" << endl;
  vector<vector<double>> points1 = {
      {0, 0},
      {1, 1},
      {2, 2},
      {10, 10},
      {0.5, 0.5}};

  cout << "Puntos:" << endl;
  for (int i = 0; i < points1.size(); i++) {
    cout << "Punto " << i << ": (" << points1[i][0] << ", " << points1[i][1] << ")" << endl;
  }

  vector<int> result1 = closest_points(points1);
  cout << "Par mas cercano: indices " << result1[0] << " y " << result1[1] << endl;

  double dist1 = sqrt(distance_squared(points1, result1[0], result1[1]));
  cout << "Distancia: " << fixed << setprecision(4) << dist1 << endl;
  cout << "Coordenadas: (" << points1[result1[0]][0] << ", " << points1[result1[0]][1] << ") y ("
       << points1[result1[1]][0] << ", " << points1[result1[1]][1] << ")" << endl;

  cout << "\n=== Caso de Prueba 2: Puntos que cruzan linea divisoria ===" << endl;
  vector<vector<double>> points2 = {
      {0, 0},
      {3, 0},
      {1.4, 1},
      {1.6, 1},
      {5, 5},
      {-2, -2}};

  cout << "Puntos:" << endl;
  for (int i = 0; i < points2.size(); i++) {
    cout << "Punto " << i << ": (" << points2[i][0] << ", " << points2[i][1] << ")" << endl;
  }

  vector<int> result2 = closest_points(points2);
  cout << "Par mas cercano: indices " << result2[0] << " y " << result2[1] << endl;

  double dist2 = sqrt(distance_squared(points2, result2[0], result2[1]));
  cout << "Distancia: " << fixed << setprecision(4) << dist2 << endl;
  cout << "Coordenadas: (" << points2[result2[0]][0] << ", " << points2[result2[0]][1] << ") y ("
       << points2[result2[1]][0] << ", " << points2[result2[1]][1] << ")" << endl;

  cout << "\n=== Caso de Prueba 3: Solo dos puntos ===" << endl;
  vector<vector<int>> points3 = {
      {-1, -1},
      {4, 5}};

  cout << "Puntos:" << endl;
  for (int i = 0; i < points3.size(); i++) {
    cout << "Punto " << i << ": (" << points3[i][0] << ", " << points3[i][1] << ")" << endl;
  }

  vector<int> result3 = closest_points(points3);
  cout << "Par mas cercano: indices " << result3[0] << " y " << result3[1] << endl;

  double dist3 = sqrt(distance_squared(points3, result3[0], result3[1]));
  cout << "Distancia: " << fixed << setprecision(4) << dist3 << endl;
  cout << "Coordenadas: (" << points3[result3[0]][0] << ", " << points3[result3[0]][1] << ") y (" << points3[result3[1]][0] << ", " << points3[result3[1]][1] << ")" << endl;

  return 0;
}