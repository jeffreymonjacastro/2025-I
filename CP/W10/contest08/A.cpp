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

void dijkstra(int src, vector<vector<pll>> &adj, vector<ll> &dist, vector<bool> &visited, vector<ll> &nways) {
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
      if (d + w == dist[v]) {
        nways[v] = nways[v] + 1;
      } else if (d + w < dist[v]) {
        nways[v] = 1;
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

  vector<ll> dist(n, LLONG_MAX);
  vector<bool> visited(n, false);
  vector<ll> nways(n, -1);

  dijkstra(0, adj, dist, visited, nways);

  for (int i = 0; i < n; i++) {
    cout << "Dist from 0 to " << i << ": " << dist[i] << ", Number of ways: " << nways[i] << endl;
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
