#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const ll mod = 1e9 + 7;

ll binPow1(ll a, ll b) {
  a %= mod - 1;
  ll res = 1;
  while (b > 0) {
    if (b & 1)
      res = (res * a) % (mod - 1);
    a = (a * a) % (mod - 1);
    b >>= 1;
  }

  return res;
}

ll binPow(ll a, ll b) {
  a %= mod;
  ll res = 1;

  while (b > 0) {
    if (b & 1)
      res = (res * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }

  return res;
}

void solve() {
  ll a, b, c;
  cin >> a >> b >> c;

  if (b == 0 && c == 0) {
    cout << a << endl;
    return;
  }

  if (a == 0 && b == 0) {
    cout << 1 << endl;
    return;
  }

  if (a == 0) {
    cout << 0 << endl;
    return;
  }

  ll z = binPow1(b, c);

  cout << binPow(a, z) << endl;
}

int main() {
  cpu();
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
