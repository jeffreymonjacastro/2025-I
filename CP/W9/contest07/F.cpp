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
  vector<string> g(n);
  for (int i = 0; i < n; i++) {
    cin >> g[i];
  }

  vector<vector<bool>> sea(n, vector<bool>(m, false));
  queue<pair<int, int>> q;

  int dx[] = {-1, 1, 0, 0};
  int dy[] = {0, 0, -1, 1};

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (g[i][j] == '0' && (i == 0 || i == n - 1 || j == 0 || j == m - 1)) {
        sea[i][j] = true;
        q.push({i, j});
      }
    }
  }

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();

    for (int k = 0; k < 4; k++) {
      int nx = x + dx[k];
      int ny = y + dy[k];

      if (nx >= 0 && nx < n && ny >= 0 && ny < m && g[nx][ny] == '0' &&
          !sea[nx][ny]) {
        sea[nx][ny] = true;
        q.push({nx, ny});
      }
    }
  }

  int sum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (g[i][j] == '1') {
        for (int k = 0; k < 4; k++) {
          int ni = i + dx[k];
          int nj = j + dy[k];

          if ((ni < 0 || ni >= n || nj < 0 || nj >= m) ||
              (g[ni][nj] == '0' && sea[ni][nj])) {
            sum++;
          }
        }
      }
    }
  }

  cout << sum << endl;
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}