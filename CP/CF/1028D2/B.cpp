#include <bits/stdc++.h>
using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define pll pair<ll, ll>
#define f first
#define s second

const int mod = 998244353;

ll binPow(ll a, ll b) {
  a %= mod;
  ll result = 1;
  while (b > 0) {
    if (b & 1LL)
      result = (result * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }
  return result;
}

void solve() {
  int n;
  cin >> n;
  vector<ll> p(n);
  vector<ll> q(n);

  for (int i = 0; i < n; i++) {
    cin >> p[i];
  }

  for (int i = 0; i < n; i++) {
    cin >> q[i];
  }

  vector<ll> r(n, 0);
  r[0] = (binPow(2, p[0]) + binPow(2, q[0])) % mod;

  pll maxp = {0, p[0]};
  pll maxq = {0, q[0]};

  for (int i = 1; i < n; i++) {
    if (p[i] > maxp.s) {
      maxp = {i, p[i]};
    }

    if (q[i] > maxq.s) {
      maxq = {i, q[i]};
    }

    if (maxp.s > maxq.s) {
      r[i] = (binPow(2, maxp.s) + binPow(2, q[i - maxp.f])) % mod;
    } else if (maxp.s < maxq.s) {
      r[i] = (binPow(2, maxq.s) + binPow(2, p[i - maxq.f])) % mod;
    } else {
      if (q[i - maxp.f] > p[i - maxq.f]) {
        r[i] = (binPow(2, maxp.s) + binPow(2, q[i - maxp.f])) % mod;
      } else {
        r[i] = (binPow(2, maxq.s) + binPow(2, p[i - maxq.f])) % mod;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    cout << r[i] << " ";
  }
  cout << "\n";
}

int main() {
  cpu();
  int t;
  t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}