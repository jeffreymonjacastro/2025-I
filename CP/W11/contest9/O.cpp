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
  ll a, b, x;
  cin >> a >> b >> x;

  if (x > max(a, b)) {
    cout << "NO" << "\n";
    return;
  }

  if (x == a || x == b) {
    cout << "YES" << "\n";
    return;
  }

  if (a < b)
    swap(a, b);

  function<bool(ll, ll)> gcd = [&](ll a, ll b) -> bool {
    if (b == 0)
      return false;

    if (a == x || b == x)
      return true;

    if (a - b == x || b - a == x)
      return true;

    return gcd(a, a - b);
  };

  if (gcd(a, b)) {
    cout << "YES" << "\n";
  } else {
    cout << "NO" << "\n";
  }
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
