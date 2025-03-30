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
  vector<vector<bool>> n(3, vector<bool>(3, true));

  int r, c, min = 1e9;
  string s;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cin >> m[i][j];
      if (m[i][j] - 'A' < min) {
        min = m[i][j] - 'A';
        r = i;
        c = j;
      }
    }
  }

  n[r][c] = false;
  s += m[r][c];
  int a, b;

  for (int i = 0; i < 2; i++) {
    min = 1e9;
    a = r;
    b = c;

    int directions[3] = {-1, 0, 1};

    for (int dr : directions) {
      for (int dc : directions) {
        if (dr == 0 && dc == 0)
          continue;

        int new_r = r + dr;
        int new_c = c + dc;

        if (new_r >= 0 && new_r <= 2 && new_c >= 0 && new_c <= 2) {
          if (m[new_r][new_c] - 'A' < min && n[new_r][new_c]) {
            min = m[new_r][new_c] - 'A';
            a = new_r;
            b = new_c;
          }
        }
      }
    }

    r = a;
    c = b;

    n[r][c] = false;
    s += m[r][c];
  }

  cout << s;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
