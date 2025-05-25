#include "visvalingam_whyatt.h"

int main() {
  vector<vector<int>> originalLine = {
      {0, 0},
      {2, 3},
      {3, 2},
      {4, 4},
      {6, 2},
      {8, 1},
      {10, 2},
  };

  printVertices(originalLine, "Linea original");

  auto simplified6 = triangles_based_simplification(originalLine, 6);
  printVertices(simplified6, "Simplificada a 6 puntos");

  auto simplified5 = triangles_based_simplification(originalLine, 5);
  printVertices(simplified5, "Simplificada a 5 puntos");

  auto simplified4 = triangles_based_simplification(originalLine, 4);
  printVertices(simplified4, "Simplificada a 4 puntos");

  auto simplified3 = triangles_based_simplification(originalLine, 3);
  printVertices(simplified3, "Simplificada a 3 puntos");

  auto simplified2 = triangles_based_simplification(originalLine, 2);
  printVertices(simplified2, "Simplificada a 2 puntos");

  return 0;
}
