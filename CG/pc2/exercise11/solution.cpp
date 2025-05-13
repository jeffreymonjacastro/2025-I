#include "area.h"

int main() {
  vector<vector<double>> vertices = {
      {0.0, 0.0}, {4.0, 0.0}, {4.0, 3.0}, {0.0, 3.0}};
  double result = area(vertices);
  cout << "Area of the polygon: " << result << endl;
  return 0;
}