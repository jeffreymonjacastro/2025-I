#include "exist_intersections.h"

int main() {
  vector<vector<int>> segments = {
      {0, 0, 2, 2},
      {1, 1, 3, 3},
      {2, 0, 0, 2},
      {3, 1, 1, 3},
      {4, 0, 0, 4},
      {5, 1, 1, 5},
      {6, 0, 0, 6},
      {7, 1, 1, 7},
      {8, 0, 0, 8},
      {9, 1, 1, 9},
      {10, 0, 0, 10},
      {11, 1, 1, 11},
      {12, 0, 0, 12},
      {13, 1, 1, 13},
      {14, 0, 0, 14},
      {15, 1, 1, 15},
  };

  bool brute_result = IntersectionDetector::detect(segments, "brute");
  bool sweep_result = IntersectionDetector::detect(segments, "sweep");

  cout << "Metodo fuerza bruta: " << (brute_result ? "SI" : "NO") << endl;
  cout << "Metodo linea de barrido: " << (sweep_result ? "SI" : "NO") << endl;

  if (brute_result == sweep_result) {
    cout << "CORRECTO" << endl;
  } else {
    cout << "ERROR - Resultados diferentes!" << endl;
  }

  return 0;
}