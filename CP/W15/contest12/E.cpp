#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

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
  lld m, r;
  cin >> m >> r;

  if (m == 1.0) {
    cout << fixed << setprecision(10) << 2.0 * r << endl;
    return;
  }
  
  lld avg = 2.0 * r * (1.0 + (m - 1.0) / m * (1.0 / sqrt(2.0)));
  
  cout << fixed << setprecision(10) << avg << endl;
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