#include "is_convex.h"

int main() {
  int n = 4;
  vector<vector<int>> P(n, vector<int>(2));
  P[0] = {0, 0};
  P[1] = {1, 0};
  P[2] = {2, 0};
  P[3] = {1, 1};

  cout << boolalpha;
  cout << is_convex(P) << endl;

  return 0;
}