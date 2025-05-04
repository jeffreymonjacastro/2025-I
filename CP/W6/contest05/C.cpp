#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  ll n, k;
  cin >> n >> k;

  lld sum = n * (2LL * k + n - 1) / 2;
  ll l = 1, r = n;

  while (l < r) {
    ll mid = (l + r) / 2;
    lld sum1 = mid * (2LL * k + mid - 1) / 2;

    if (2 * sum1 > sum) {
      r = mid;
    } else {
      l = mid + 1;
    }
  }

  ll ans;
  if (l > 1) {
    ll sum1 = l * (2LL * k + l - 1) / 2;
    ll sum2 = (l - 1) * (2LL * k + (l - 1) - 1) / 2;
    ll z1 = abs(2 * sum1 - sum);
    ll z2 = abs(2 * sum2 - sum);
    ans = min(z1, z2);
  } else {
    ll sum1 = l * (2LL * k + l - 1) / 2;
    ans = abs(2 * sum1 - sum);
  }

  cout << ans << endl;
}

int main() {
  cpu();
  int t;
  t = 1;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
