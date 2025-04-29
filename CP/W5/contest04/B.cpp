#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const ll mod = 1e9 + 7;

void solve() {
  int n;
  cin >> n;

  vector<ll> dp = {1, 1, 1, 0};
  vector<ll> xd(4);

  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < 4; j++) {
      xd[j] = -dp[j];
      for (int z = 0; z < 4; z++) {
        xd[j] += dp[z];
      }

      xd[j] = xd[j] % mod;
    }

    dp = xd;
  }

  cout << dp[3];
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
