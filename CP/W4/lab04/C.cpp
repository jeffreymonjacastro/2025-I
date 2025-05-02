#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

// ll pot(int x, ll n) {
//   if (n == 0)
//     return 1;

//   ll y = pot(x, n / 2);
//   y = (y * y) % mod;
//   if (n % 2)
//     y = (y * x) % mod;
//   return y;
// }

ll pot(int x, ll n) {
  ll result = 1;
  x %= mod;

  while (n > 0) {
    // If n is odd, multiply result with x
    if (n % 2 == 1) {
      result = (result * x) % mod;
    }

    // n = n/2
    n >>= 1; // Equivalent to n = n / 2

    // x = x^2
    x = (1LL * x * x) % mod;
  }

  return result;
}

void solve() {
  ll n;
  cin >> n;

  if (n == 0 || n == 1) {
    cout << 1 << endl;
    return;
  }

  if (n % 3 == 0) {
    cout << pot(3, n / 3) % mod << endl;
  } else if (n % 3 == 1) {
    cout << (pot(3, (n - 4) / 3) * 4) % mod << endl;
  } else {
    cout << (pot(3, (n - 2) / 3) * 2) % mod << endl;
  }
}

int main() {
  cpu();
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
