#include <bits/stdc++.h>

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

  vector<vector<int>> neighbors(n + 1);

  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  map<int, int> grado;

  for (int i = 1; i <= n; i++) {
    grado[neighbors[i].size()]++;
  }

  int x = 0;
  int y = 0;

  if (grado.size() == 2) {
    for (auto [u, v] : grado) {
      if (u != 1) {
        x = u;
        y = u - 1;
      }
    }
  }

  for (auto [u, v] : grado) {
    if (v == 1) {
      x = u;
    } else if (u != 1) {
      y = u - 1;
    }
  }

  cout << x << " " << y << endl;
}

int main() {
  cpu();
  int t;
  t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
