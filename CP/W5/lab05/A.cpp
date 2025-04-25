#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int W, N;
  cin >> W >> N;

  vector<int> w(N);
  vector<int> v(N);

  for (int i = 0; i < N; i++) {
    cin >> w[i] >> v[i];
  }

  vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= W; j++) {
      if (j >= w[i - 1]) {
        dp[i][j] = max(dp[i - 1][j], v[i - 1] + dp[i - 1][j - w[i - 1]]);
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }

  cout << dp[N][W];
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
