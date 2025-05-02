#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  string s;
  cin >> s;

  int n = s.size();
  vector<vector<bool>> dp(n + 1, vector<bool>(9, false));

  dp[0][0] = true;

  // DP
  for (int i = 1; i <= n; i++) {
    int digit = s[i - 1] - '0';
    for (int r = 0; r < 9; r++) {
      if (dp[i - 1][r]) {
        dp[i][(r + digit) % 9] = true;
        if (digit * digit < 10) {
          dp[i][(r + digit * digit) % 9] = true;
        }
      }
    }
  }

  if (dp[n][0]) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
  }
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
