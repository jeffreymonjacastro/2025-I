#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define pii pair<int, int>
#define f first
#define s second

const int mod = 1e9 + 7;

void solve() {
  int n;
  cin >> n;

  int parent[n + 1];

  auto make_set = [&](int v) {
    parent[v] = v;
  };

  function<int(int)> find_set = [&](int v) {
    if (v == parent[v])
      return v;
    return find_set(parent[v]);
  };

  auto union_sets = [&](int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      parent[b] = a;
      return true;
    }
    return false;
  };

  for (int i = 1; i <= n; i++) {
    make_set(i);
  }

  vector<pii> close;
  vector<pii> build;

  int u, v;
  for (int i = 1; i < n; i++) {
    cin >> u >> v;

    if (!union_sets(u, v)) {
      close.push_back({u, v});
    }
  }

  for (int i = 1; i <= n; i++) {
    for (int j = i + 1; j <= n; j++) {
      if (find_set(i) != find_set(j)) {
        union_sets(i, j);
        build.push_back({i, j});
      }
    }
  }

  cout << close.size() << "\n";

  for (int i = 0; i < close.size(); i++) {
    cout << close[i].f << " " << close[i].s << " " << build[i].f << " " << build[i].s << "\n";
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
