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
  int n;
  cin >> n;
  vector<ll> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  int maxLen = 1;

  for (int i = 0; i < n; i++) {
    int len = 1;
    ll current = a[i];

    for (int j = i + 1; j < n; j++) {
      if (a[j] <= current * 2) {
        current = a[j];
        len++;
      }
    }

    maxLen = max(maxLen, len);
  }

  cout << maxLen << "\n";
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