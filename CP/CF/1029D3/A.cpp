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
  int n, x;
  cin >> n >> x;

  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  if (x >= n) {
    cout << "YES\n";
    return;
  }

  bool flag = true;

  for (int i = 0; i < n; i++) {
    if (a[i] == 0) {
      continue;
    } else if (a[i] == 1 && flag) {
      i += x - 1;
      flag = false;
    } else if (a[i] == 1 && !flag) {
      cout << "NO\n";
      return;
    }
  }

  cout << "YES\n";
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
