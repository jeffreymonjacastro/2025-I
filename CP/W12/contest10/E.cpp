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

const ll MAXN = 1e5;

struct Node {
  int v;
  int c;
  int gcd;
};

Node t[4 * MAXN];

int left(int v) {
  return v * 2;
}

int right(int v) {
  return v * 2 + 1;
}

Node combine(Node a, Node b) {
  int new_gcd = gcd(a.gcd, b.gcd);

  if (a.v > b.v)
    return {b.v, b.c, new_gcd};
  if (a.v < b.v)
    return {a.v, a.c, new_gcd};
  return {a.v, a.c + b.c, new_gcd};
}

void build(int a[], int v, int tl, int tr) {
  if (tl == tr) {
    t[v] = {a[tl], 1, a[tl]};
  } else {
    int tm = (tl + tr) / 2;
    build(a, left(v), tl, tm);
    build(a, right(v), tm + 1, tr);
    t[v] = combine(t[left(v)], t[right(v)]);
  }
}

void update(int v, int tl, int tr, int pos, int new_val) {
  if (tl == tr) {
    t[v] = {new_val, 1, new_val};
  } else {
    int tm = (tl + tr) / 2;
    if (pos <= tm)
      update(left(v), tl, tm, pos, new_val);
    else
      update(right(v), tm + 1, tr, pos, new_val);
    t[v] = combine(t[left(v)], t[right(v)]);
  }
}

Node get_min(int v, int tl, int tr, int l, int r) {
  if (l > r) {
    return {INT_MAX, 0, 0};
  }
  if (l == tl && r == tr) {
    return t[v];
  }
  int tm = (tl + tr) / 2;
  Node left_min = get_min(left(v), tl, tm, l, min(r, tm));
  Node right_min = get_min(right(v), tm + 1, tr, max(l, tm + 1), r);
  return combine(left_min, right_min);
}

void solve() {
  int n, q, l, r;
  cin >> n;

  int a[n + 1];
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  build(a, 1, 1, n);

  cin >> q;

  while (q--) {
    cin >> l >> r;
    Node res = get_min(1, 1, n, l, r);

    if (res.v == res.gcd) {
      cout << r - l + 1 - res.c << "\n";
    } else {
      cout << r - l + 1 << "\n";
    }
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
