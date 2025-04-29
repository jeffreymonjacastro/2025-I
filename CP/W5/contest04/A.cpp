#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n;
  cin >> n;
  vector<string> s(n);
  vector<vector<int>> dp(n, vector<int>(n, 0));

  for (int i = 0; i < n; i++) {
    cin >> s[i];
  }

  if (s[0][0] == '*' || s[n - 1][n - 1] == '*') {
    cout << 0;
    return;
  }

  dp[0][0] = 1;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '*')
        continue;
      if (i > 0) {
        dp[i][j] = (dp[i][j] + dp[i - 1][j]) % mod;
      }
      if (j > 0) {
        dp[i][j] = (dp[i][j] + dp[i][j - 1]) % mod;
      }
    }
  }

  cout << dp[n - 1][n - 1];
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
