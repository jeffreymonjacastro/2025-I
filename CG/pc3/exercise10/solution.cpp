#include "solution.h"

int main() {
  std::cout << "=== Test de Marching Squares Adaptativo ===" << std::endl;

  // Ejemplo 1: Un círculo simple
  std::string circle_json = R"({
        "op": "shape",
        "function": "(x-2)^2+(y-2)^2-1",
        "childs": []
    })";

  std::cout << "Generando circulo..." << std::endl;
  marching_squares(circle_json, "circle_test.eps", -5, -5, 6, 6, 0.1);

  // Ejemplo 2: Unión de dos círculos
  std::string union_json = R"({
        "op": "union",
        "function": "",
        "childs": [
            {
                "op": "shape",
                "function": "(x-2)^2+(y-2)^2-1",
                "childs": []
            },
            {
                "op": "shape", 
                "function": "(x-4)^2+(y-2)^2-1",
                "childs": []
            }
        ]
    })";

  std::cout << "Generando union de circulos..." << std::endl;
  marching_squares(union_json, "union_test.eps", -5, -5, 8, 6, 0.1);

  // Ejemplo 3: Círculo más complejo
  std::string complex_json = R"({
        "op": "union",
        "function": "",
        "childs": [
            {
                "op": "shape",
                "function": "(x-220)^2 + (y-200)^2 - 80^2",
                "childs": []
            },
            {
                "op": "shape",
                "function": "(x-180)^2 + (y-200)^2 - 80^2", 
                "childs": []
            }
        ]
    })";

  std::cout << "Generando circulos complejos..." << std::endl;
  marching_squares(complex_json, "complex_test.eps", 0, 0, 400, 400, 2.0);

  // Ejemplo 4: Diferencia (sustracción)
  std::string diff_json = R"({
        "op": "diff",
        "function": "",
        "childs": [
            {
                "op": "shape",
                "function": "(x-3)^2+(y-3)^2-4",
                "childs": []
            },
            {
                "op": "shape",
                "function": "(x-3)^2+(y-3)^2-1",
                "childs": []
            }
        ]
    })";

  std::cout << "Generando diferencia (anillo)..." << std::endl;
  marching_squares(diff_json, "diff_test.eps", -2, -2, 8, 8, 0.1);

  // Ejemplo 5: Intersección
  std::string intersection_json = R"({
        "op": "intersection",
        "function": "",
        "childs": [
            {
                "op": "shape",
                "function": "(x-2)^2+(y-2)^2-4",
                "childs": []
            },
            {
                "op": "shape",
                "function": "(x-4)^2+(y-2)^2-4",
                "childs": []
            }
        ]
    })";

  std::cout << "Generando interseccion..." << std::endl;
  marching_squares(intersection_json, "intersection_test.eps", -2, -2, 8, 6, 0.1);

  return 0;
}