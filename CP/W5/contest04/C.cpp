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
  vector<int> h(n);

  for (int i = 0; i < n; i++) {
    cin >> h[i];
  }

  vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(n, 0)));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int l = i; l < j; k++) {
        dp[i][j][k] = min(dp[i][j][k], dp[i][l][k])
      }
    }
  }
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
