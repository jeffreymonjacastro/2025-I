#include "inside_polygon.h"

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