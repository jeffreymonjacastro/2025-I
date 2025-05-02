#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

ll binPow(ll a, ll b) {
  a %= mod;
  ll result = 1;

  while (b > 0) {
    if (b & 1)
      result = (result * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }

  return result;
}

void solve() {
  ll a, b;
  cin >> a >> b;

  if (a == 0 && b == 0) {
    cout << 1 << endl;
    return;
  }

  if (a == 0) {
    cout << 0 << endl;
    return;
  }

  cout << binPow(a, b) << endl;
}

int main() {
  cpu();
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
