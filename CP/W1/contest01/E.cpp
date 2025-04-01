#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  vector<vector<char>> m(3, vector<char>(3));
  string s, sol;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cin >> m[i][j];
    }
  }

  int directions[3] = {-1, 0, 1};

  sol = "ZZZ";

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      for (int firstdr : directions) {
        for (int firstdc : directions) {
          int firstr = r + firstdr;
          int firstc = c + firstdc;

          if ((firstdr == 0 && firstdc == 0) || (firstr == r && firstc == c) ||
              firstr < 0 || firstr > 2 || firstc < 0 || firstc > 2)
            continue;

          for (int seconddr : directions) {
            for (int seconddc : directions) {
              int secondr = firstr + seconddr;
              int secondc = firstc + seconddc;

              if ((seconddr == 0 && seconddc == 0) ||
                  (secondr == r && secondc == c) || secondr < 0 ||
                  secondr > 2 || secondc < 0 || secondc > 2)
                continue;

              s = string(1, m[r][c]) + string(1, m[firstr][firstc]) +
                  string(1, m[secondr][secondc]);

              if (s < sol) {
                sol = s;
              }
            }
          }
        }
      }
    }
  }

  cout << sol;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
