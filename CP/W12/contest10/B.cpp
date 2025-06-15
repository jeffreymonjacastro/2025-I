#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const ll MAXN = 2 * 1e5;

struct cosa {
  ll sum, pref, suff, ans;
};

cosa t[4 * MAXN];

cosa combine(cosa l, cosa r) {
  cosa res;
  res.sum = l.sum + r.sum;
  res.pref = max(l.pref, l.sum + r.pref);
  res.suff = max(r.suff, r.sum + l.suff);
  res.ans = max(max(l.ans, r.ans), l.suff + r.pref);
  return res;
}

cosa make_cosa(ll val) {
  cosa res;
  res.sum = val;
  res.pref = res.suff = res.ans = max(0LL, val);
  return res;
}

ll left(ll v) {
  return v * 2;
}

ll right(ll v) {
  return v * 2 + 1;
}

void build(ll a[], ll v, ll tl, ll tr) {
  if (tl == tr) {
    t[v] = make_cosa(a[tl]);
  } else {
    ll tm = (tl + tr) / 2;
    build(a, left(v), tl, tm);
    build(a, right(v), tm + 1, tr);
    t[v] = combine(t[left(v)], t[right(v)]);
  }
}

void update(ll v, ll tl, ll tr, ll pos, ll new_val) {
  if (tl == tr) {
    t[v] = make_cosa(new_val);
  } else {
    ll tm = (tl + tr) / 2;
    if (pos <= tm)
      update(left(v), tl, tm, pos, new_val);
    else
      update(right(v), tm + 1, tr, pos, new_val);
    t[v] = combine(t[left(v)], t[right(v)]);
  }
}

cosa query(ll v, ll tl, ll tr, ll l, ll r) {
  if (l > r)
    return make_cosa(0);
  if (l == tl && r == tr) {
    return t[v];
  }
  ll tm = (tl + tr) / 2;
  return combine(query(left(v), tl, tm, l, min(r, tm)), query(right(v), tm + 1, tr, max(l, tm + 1), r));
}

void solve() {
  ll n, m;
  cin >> n >> m;

  ll a[n];
  for (ll i = 0; i < n; i++) {
    cin >> a[i];
  }

  build(a, 1, 0, n - 1);

  ll x, y;
  for (ll i = 0; i < m; i++) {
    cin >> x >> y;
    update(1, 0, n - 1, x - 1, y);
    cosa res = query(1, 0, n - 1, 0, n - 1);
    cout << res.ans << "\n";
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
