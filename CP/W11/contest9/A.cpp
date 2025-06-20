#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  ll a, b, n, x, vx, vy, u, ans = 0, sz, tot, cnt;
  cin >> n >> a >> b;
  unordered_map<ll, vector<ll>> m;
  m.reserve(n * 2);

  for (int i = 0; i < n; i++) {
    cin >> x >> vx >> vy;
    u = vy - a * vx;
    m[u].push_back(vx);
  }

  for (auto &p : m) {
    auto &v = p.second;
    sz = v.size();
    if (sz < 2)
      continue;
    tot = sz * (sz - 1LL) / 2;
    unordered_map<ll, ll> freq;
    for (auto gv : v)
      freq[gv]++;
    cnt = 0;
    for (auto &q : freq)
      cnt += q.second * (q.second - 1) / 2;
    ans += (tot - cnt);
  }
  cout << 2 * ans << "\n";
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
