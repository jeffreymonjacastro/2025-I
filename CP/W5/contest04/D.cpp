#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, k;
  cin >> n >> k;

  vector<int> h(n);
  vector<int> dp(n, INT_MAX);
  dp[0] = 0;

  for (int i = 0; i < n; i++) {
    cin >> h[i];
  }

  for (int i = 1; i < n; i++) {
    for (int j = 1; j <= k; j++) {
      if (i - j >= 0) {
        dp[i] = min(dp[i], dp[i - j] + abs(h[i] - h[i - j]));
      }
    }
  }

  cout << dp[n - 1];
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
