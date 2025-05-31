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

void dijkstra(int src, vector<vector<pll>> &adj, vector<ll> &dist, vector<bool> &visited) {
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
        pq.push({dist[v], v});
      }
    }
  }
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<vector<pll>> adj(n);

  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }

  vector<ll> dist1(n, LLONG_MAX);
  vector<bool> visited1(n, false);

  dijkstra(0, adj, dist1, visited1);

  vector<ll> dist2(n, LLONG_MAX);
  vector<bool> visited2(n, false);

  dijkstra(n - 1, adj, dist2, visited2);

  int min = dist1[n - 1];

  int total = 0;

  for (int u = 0; u < n; u++) {
    for (auto &[v, w] : adj[u]) {
      if (dist1[u] + w + dist2[v] == min) {
        total += w;
      }
    }
  }

  cout << 2 * total;
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
