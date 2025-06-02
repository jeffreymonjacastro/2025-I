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

void dijkstra(int src, vector<vector<pll>> &adj, vector<ll> &dist, vector<bool> &visited, vector<ll> &parent) {
  priority_queue<pll, vector<pll>, greater<pll>> pq;
  dist[src] = 0;
  pq.push({0, src});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (visited[u])
      continue;

    visited[u] = true;

    if (d > dist[u])
      continue;

    for (auto &[v, w] : adj[u]) {
      if (d + w < dist[v]) {
        dist[v] = d + w;
        parent[v] = u;
        pq.push({dist[v], v});
      }
    }
  }
}

void print_path(int u, vector<ll> &parent) {
  if (u == -1) {
    return;
  }
  print_path(parent[u], parent);
  cout << u << " ";
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<vector<pll>> adj(n + 1);

  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }

  vector<ll> dist(n + 1, LLONG_MAX);
  vector<bool> visited(n + 1, false);
  vector<ll> parent(n + 1, -1);

  dijkstra(1, adj, dist, visited, parent);

  if (dist[n] == LLONG_MAX) {
    cout << "-1";
    return;
  }

  print_path(n, parent);
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
