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

  vector<int> a(n);

  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  vector<set<int>> suffix(n);

  for (int i = n - 1; i >= 0; i--) {
    suffix[i].insert(a[i]);
    if (i < n - 1) {
      suffix[i].insert(suffix[i + 1].begin(), suffix[i + 1].end());
    }
  }

  int cnt = 1;
  set<int> curr;
  
  for (int i = n - 1; i >= 0; i--) {

  }

  cout << cnt << "\n";
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
