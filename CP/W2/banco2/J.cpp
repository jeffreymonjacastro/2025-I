#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, m;
  cin >> n >> m;
  string s;

  vector<vector<int>> rings(n, vector<int>(m));

  for (int i = 0; i < n; i++) {
    cin >> s;
    for (int j = 0; j < m; j++) {
      if (s[j] == 'T') {
        rings[i][j] = 1;
      } else {
        rings[i][j] = 0;
      }
    }
  }

  int max_ring = min(ceil(n / 2.0), ceil(m / 2.0));

  for (int k = 1; k < max_ring; k++) {
    int new_n = n - (k + 1);
    int new_m = m - (k + 1);

    for (int i = k; i <= new_n; i++) {
      for (int j = k; j <= new_m; j++) {
        if (rings[i - 1][j] >= k && rings[i][j - 1] >= k &&
            rings[i + 1][j] >= k && rings[i][j + 1] >= k) {
          rings[i][j]++;
        }
      }
    }
  }

  if (max_ring / 10 == 0) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        if (rings[i][j] > 0) {
          cout << "." << rings[i][j];
        } else {
          cout << "..";
        }
      }
      cout << endl;
    }
  } else {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (rings[i][j] == 0) {
          cout << "...";
        } else if (rings[i][j] / 10 == 0) {
          cout << ".." << rings[i][j];
        } else {
          cout << "." << rings[i][j];
        }
      }
      cout << endl;
    }
  }
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  solve();
  return 0;
}
