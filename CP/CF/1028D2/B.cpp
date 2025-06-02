#include <bits/stdc++.h>
using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
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

  vector<ll> dp(n, 0);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
        }
  }

  for (int i = 0; i < n; i++) {
    cout << dp[i] << " ";
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