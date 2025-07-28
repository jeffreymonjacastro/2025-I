#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;
const ll MOD = 998244353;

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
  ll n;
  cin >> n;
  string s;
  cin >> s;

  s = " " + s;
  ll ans = 0, cur = 1;
  for (ll i = 1; i <= n; i++) {
    if (s[i] == s[i - 1]) {
      cur = (2 * cur) % MOD;
    } else {
      cur = 1;
    }
    ans = (ans + cur) % MOD;
  }
  cout << ans << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  ll t;
  cin >> t;
  while (t--) {
    solve();
  }
}