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
  int n, l;
  cin >> n >> l;

  vector<int> a(n - 1);
  map<int, int> circular;

  for (int i = 0; i < n - 1; i++) {
    cin >> a[i];
  }

  int count = 0;

  for (int i = 0; i < n - 1; i++) {
    if (a[i] < l) {
      count++;
    }
  }

  cout << count << "\n";
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
