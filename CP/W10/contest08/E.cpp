#include <bits/stdc++.h>
using namespace std;

#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

bool colors(ll u, vector<vector<ll>> &adj, vector<ll> &color) {
  for (ll v : adj[u]) {
    if (color[v] == -1) {
      color[v] = color[u] ^ 1;
      if (!colors(v, adj, color))
        return false;
    } else if (color[v] == color[u])
      return false;
  }
  return true;
};

void solve() {
  ll n, m, u, v;
  string a, b, g1, g2;
  cin >> n;

  vector<vector<ll>> adj(n, vector<ll>());
  vector<string> items(n);
  unordered_map<string, ll> idx;

  for (ll i = 0; i < n; i++) {
    cin >> items[i];
    idx[items[i]] = i;
  }

  cin >> m;

  for (ll i = 0; i < m; i++) {
    cin >> a >> b;
    u = idx[a], v = idx[b];
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  vector<ll> color(n, -1);
  bool flag = true;

  for (ll i = 0; i < n; i++) {
    if (color[i] == -1) {
      color[i] = 0;
      if (!colors(i, adj, color)) {
        flag = false;
        break;
      }
    }
  }

  if (!flag) {
    cout << "impossible\n";
    return;
  }

  for (ll u = 0; u < n; u++) {
    if (color[u] == 0)
      g1 += items[u] + " ";
    else
      g2 += items[u] + " ";
  }

  if (!g1.empty() && g1.back() == ' ')
    g1.pop_back();
  if (!g2.empty() && g2.back() == ' ')
    g2.pop_back();

  cout << g1 << endl;
  cout << g2;
};

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
