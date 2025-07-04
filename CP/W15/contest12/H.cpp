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
  int n, w;
  cin >> n >> w;
  
  vector<int> a(n), b(w);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  for (int i = 0; i < w; i++) {
    cin >> b[i];
  }
  
  if (w == 1) {
    cout << n << "\n";
    return;
  }
  
  if (w > n) {
    cout << 0 << "\n";
    return;
  }
  
  vector<int> elephant_diff(w - 1);
  for (int i = 0; i < w - 1; i++) {
    elephant_diff[i] = b[i + 1] - b[i];
  }
  
  int count = 0;
  for (int i = 0; i <= n - w; i++) {
    bool matches = true;
    for (int j = 0; j < w - 1; j++) {
      if (a[i + j + 1] - a[i + j] != elephant_diff[j]) {
        matches = false;
        break;
      }
    }
    if (matches) {
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