#include <bits/stdc++.h>
using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

int dfs(int node, int parent, int gatos, int m, vector<int> &a, vector<vector<int>> &adj) {
  if (a[node] == 1)
    gatos++;
  else
    gatos = 0;

  if (gatos > m)
    return 0;

  int restaurantes = 0;
  bool hoja = true;

  for (int vecino : adj[node]) {
    if (vecino != parent) {
      hoja = false;
      restaurantes += dfs(vecino, node, gatos, m, a, adj);
    }
  }

  if (hoja)
    return 1;
  return restaurantes;
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<int> a(n + 1, 0);
  vector<vector<int>> adj(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  int u, v;
  for (int i = 0; i < n - 1; i++) {
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  cout << dfs(1, -1, 0, m, a, adj) << endl;
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