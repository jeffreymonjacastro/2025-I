#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 10000007;

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

void solve(ll n, ll k) {
  cout << ((2 * binPow(n - 1, k)) + binPow(n, k) + (2 * binPow(n - 1, n - 1)) +
           binPow(n, n)) %
              mod
       << endl;
}

int main() {
  cpu();
  ll n, k;
  while (true) {
    cin >> n >> k;
    if (n == 0 || k == 0)
      break;
    solve(n, k);
  }
  return 0;
}
