#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const ll MAXN = 2 * 1e5;

ll t[4 * MAXN];

ll left(ll v) {
  return v * 2;
}

ll right(ll v) {
  return v * 2 + 1;
}

void build(ll a[], ll v, ll tl, ll tr) {
  if (tl == tr) {
    t[v] = a[tl];
  } else {
    ll tm = (tl + tr) / 2;
    build(a, left(v), tl, tm);
    build(a, right(v), tm + 1, tr);
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

void update(ll v, ll tl, ll tr, ll pos, ll new_val) {
  if (tl == tr) {
    t[v] = new_val;
  } else {
    ll tm = (tl + tr) / 2;
    if (pos <= tm)
      update(left(v), tl, tm, pos, new_val);
    else
      update(right(v), tm + 1, tr, pos, new_val);
    t[v] = t[left(v)] + t[right(v)];
  }
}

void solve() {
  ll n, q;
  cin >> n >> q;

  ll a[n];
  for (ll i = 0; i < n; i++) {
    cin >> a[i];
  }

  build(a, 1, 0, n - 1);

  ll x, y, z;
  for (ll i = 0; i < q; i++) {
    cin >> x >> y >> z;

    if (x == 1) {
      update(1, 0, n - 1, y - 1, z);
    } else if (x == 2) {
      cout << sum(1, 0, n - 1, y - 1, z - 1) << "\n";
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
