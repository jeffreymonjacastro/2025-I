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

void solve() {
  string s, t;
  cin >> s >> t;

  cout << s << t;

  int n;
  cin >> n;

  vector<vector<pll>> adj(26);

  char a, b;
  int w;

  for (int i = 0; i < n; i++) {
    cin >> a >> b >> w;
    adj[a - 'a'].push_back({b - 'a', w});
  }

  vector<vector<ll>> dist(26, vector<ll>(26, LLONG_MAX));

  for (int i = 0; i < 26; i++) {
    dist[i][i] = 0;
    for (auto [v, w] : adj[i]) {
      dist[i][v] = min(dist[i][v], w);
    }
  }

  for (int k = 0; k < 26; k++) {
    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        if (dist[i][k] != LLONG_MAX && dist[k][j] != LLONG_MAX) {
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  ll ans = 0;
  string res = s;

  for (int i = 0; i < s.size(); i++) {
    if (s[i] == t[i])
      continue;

    int a = s[i] - 'a';
    int b = t[i] - 'a';

    ll min_cost = LLONG_MAX;
    char best_char = 'a';

    for (int j = 0; j < 26; j++) {
      ll cost = dist[a][j] + dist[b][j];
      if (cost < min_cost) {
        min_cost = cost;
        best_char = 'a' + j;
      }
    }

    if (min_cost == LLONG_MAX) {
      cout << -1 << endl;
      return;
    }

    ans += min_cost;
    res[i] = best_char;
  }

  cout << ans << endl;
  cout << res << endl;
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
