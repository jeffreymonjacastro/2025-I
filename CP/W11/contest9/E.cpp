#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;
const double eps = 1e-6;

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
  ll n, d, h, temp;
  lld b, ans, inter;
  cin >> n >> d >> h;

  vector<ll> v(n);

  for (int i = 0; i < n; i++)
    cin >> v[i];

  b = (lld)d * h / 2.0;
  ans = n * b;

  for (int i = 1; i < n; i++) {
    temp = v[i] - v[i - 1];
    if (temp < h) {
      inter = (lld)d * (h - temp) * (h - temp) / (2.0 * h);
      ans -= inter;
    }
  }
  cout << fixed << setprecision(7) << ans << "\n";
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
