#include "inside_triangle.h"

int main() {
  int n = 4;
  vector<vector<int>> vertices(n, vector<int>(2));
  vertices[0] = {0, 0};
  vertices[1] = {2, 0};
  vertices[2] = {1, 1};

  cout << boolalpha;
  cout << inside_triangle(vertices, 1, 0) << endl;

  return 0;
}