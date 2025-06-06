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
  vector<int> dist(n + 1, INT_MAX);
  vector<int> parent(n + 1, -1);
  vector<bool> visited(n + 1, false);

  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  queue<int> q;
  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      dist[i] = 0;
      visited[i] = true;
      q.push(i);
      while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : neighbors[node]) {
          if (!visited[neighbor]) {
            visited[neighbor] = true;
            dist[neighbor] = dist[node] + 1;
            parent[neighbor] = node;
            q.push(neighbor);
          }
        }
      }
    }
  }
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
