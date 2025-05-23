#include "jarvis_graham.h"

int main() {
  int n = 6;
  vector<vector<int>> vertices(n, vector<int>(2));

  vertices[0] = {0, 1};
  vertices[1] = {1, 2};
  vertices[2] = {2, 1};
  vertices[3] = {1, 0};
  vertices[4] = {3, 1};
  vertices[5] = {2, 0};

  graham(vertices, false);
}