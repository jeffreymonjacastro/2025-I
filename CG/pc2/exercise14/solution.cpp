#include "visvalingam_whyatt.h"

int main() {
  cout << "=== Algoritmo de Visvalingam-Whyatt (Plantilla Específica) ===\n\n";

  // Ejemplo con double
  vector<vector<double>> originalLine = {
      {0.0, 0.0}, {1.0, 1.0}, {1.1, 1.05}, {2.0, 0.0}, {3.0, 2.0}, {3.05, 2.1}, {4.0, 0.0}, {5.0, 1.0}, {6.0, 0.0}, {7.0, 2.0}, {8.0, 0.0}};

  printVertices(originalLine, "Línea original (double)");

  // Simplificar a diferentes números de puntos
  auto simplified6 = triangles_based_simplification(originalLine, 6);
  printVertices(simplified6, "Simplificada a 6 puntos");

  auto simplified4 = triangles_based_simplification(originalLine, 4);
  printVertices(simplified4, "Simplificada a 4 puntos");

  auto simplified3 = triangles_based_simplification(originalLine, 3);
  printVertices(simplified3, "Simplificada a 3 puntos");

  return 0;
}