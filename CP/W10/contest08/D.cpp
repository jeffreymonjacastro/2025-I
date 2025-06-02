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
  int n;
  cin >> n;
  vector<vector<int>> dist(n, vector<int>(n));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> dist[i][j];
    }
  }

  vector<int> vertex(n);

  for (int i = 0; i < n; i++) {
    cin >> vertex[i];
    vertex[i]--;
  }
  reverse(vertex.begin(), vertex.end());

  vector<bool> active(n, false);

  function<ll()> sumar = [&]() {
    ll suma = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (active[i] && active[j] && i != j) {
          suma += dist[i][j];
        }
      }
    }
    return suma;
  };

  vector<ll> res;

  for (int k : vertex) {
    active[k] = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }

    res.push_back(sumar());
  }

  reverse(res.begin(), res.end());

  for (ll x : res) {
    cout << x << " ";
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
