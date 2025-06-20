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

bool foo(ll a, ll b, ll x) {
  if (a < b)
    swap(a, b);
  if (a == x || b == x)
    return true;
  if (x > a)
    return false;
  while (true) {
    if (b == 0)
      return a == x;
    if (x % b == a % b)
      return true;
    a %= b;
    if (a < b)
      swap(a, b);
    if (x > a)
      return false;
    if (a == x || b == x)
      return true;
  }
  return false;
}

void solve() {
  ll a, b, x;
  cin >> a >> b >> x;

  cout << (foo(a, b, x) ? "YES" : "NO") << "\n";
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
