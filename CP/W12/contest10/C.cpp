#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const ll MAXN = 1e6;

ll t[4 * MAXN];

ll left(ll v) {
  return v * 2;
}

ll right(ll v) {
  return v * 2 + 1;
}

void build(ll v, ll tl, ll tr) {
  if (tl == tr) {
    t[v] = 0;
  } else {
    ll tm = (tl + tr) / 2;
    build(left(v), tl, tm);
    build(right(v), tm + 1, tr);
    t[v] = t[left(v)] + t[right(v)];
  }
}

ll sum(ll v, ll tl, ll tr, ll l, ll r) {
  if (l > r)
    return 0;
  if (l == tl && r == tr) {
    return t[v];
  }
  ll tm = (tl + tr) / 2;
  return sum(left(v), tl, tm, l, min(r, tm)) + sum(right(v), tm + 1, tr, max(l, tm + 1), r);
}

void update(ll v, ll tl, ll tr, ll pos) {
  if (tl == tr) {
    t[v] = (t[v] == 0) ? 1 : 0;
  } else {
    ll tm = (tl + tr) / 2;
    if (pos <= tm)
      update(left(v), tl, tm, pos);
    else
      update(right(v), tm + 1, tr, pos);
    t[v] = t[left(v)] + t[right(v)];
  }
}

void solve() {
  ll n, k;
  cin >> n >> k;

  build(1, 0, n - 1);

  char op;
  ll x, y;
  for (ll i = 0; i < k; i++) {
    cin >> op;
    if (op == 'F') {
      cin >> x;
      update(1, 0, n - 1, x - 1);
    } else if (op == 'C') {
      cin >> x >> y;
      cout << sum(1, 0, n - 1, x - 1, y - 1) << "\n";
    }
  }
}

int main() {
  cpu();
  ll t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
