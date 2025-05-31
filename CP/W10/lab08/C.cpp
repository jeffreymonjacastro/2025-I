#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define pii pair<int, int>
#define pll pair<ll, ll>
#define f first
#define s second

const int mod = 1e9 + 7;

void BFS(int n, vector<vector<int>> &adj, vector<bool> &visited) {
  int n_components = 0, max_component = 0;

  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      queue<int> q;
      q.push(i);
      visited[i] = true;
      n_components++;
      int component_size = 0;
      while (!q.empty()) {
        int curr = q.front();
        q.pop();
        component_size++;

        for (int u : adj[curr]) {
          if (!visited[u]) {
            visited[u] = true;
            q.push(u);
          }
        }
      }
      max_component = max(max_component, component_size);
    }
  }
  cout << n_components << " " << max_component << "\n";
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<vector<int>> adj(n + 1);

  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);

    vector<bool> visited(n + 1, false);
    BFS(n, adj, visited);
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
