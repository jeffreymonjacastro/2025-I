#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const ll mod = 1e9 + 7;

void solve(int n) {
  vector<int> v(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  vector<int> dp(n, 0);

  for (int i = 0; i < n; i++) {
    dp[i] = 1;
    for (int j = 0; j < i; j++) {
      if (v[j] < v[i])
        dp[i] = max(dp[i], dp[j] + 1);
    }
  }

  int val = *max_element(dp.begin(), dp.end());
  cout << val << " ";

  vector<int> res;

  for (int i = n - 1; i >= 0; i--) {
    if (dp[i] == val) {
      res.push_back(v[i]);
      val--;
    }
  }

  reverse(res.begin(), res.end());

  for (int i : res) {
    cout << i << " ";
  }

  cout << endl;
}

int main() {
  cpu();
  int t;
  while (true) {
    cin >> t;
    if (t == 0)
      break;
    solve(t);
  }
  return 0;
}
