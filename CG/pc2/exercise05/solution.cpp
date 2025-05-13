#include "inside_polygon.h"

int main() {
  int n = 7;
  vector<vector<int>> vertices(n, vector<int>(2));
  vertices[0] = {4, 2};
  vertices[1] = {8, 2};
  vertices[2] = {8, 4};
  vertices[3] = {10, 4};
  vertices[4] = {6, 8};
  vertices[5] = {2, 4};
  vertices[6] = {4, 4};

  cout << boolalpha;
  cout << inside_polygon(vertices, 6, 4) << endl;

  return 0;
}