#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;
const ll maxi = 1e18 + 1;

void solve() {
  ll n, k;
  cin >> n >> k;

  vector<ll> fib(n + 1, 0);
  fib[1] = 1;
  fib[2] = 1;

  for (int i = 3; i <= n; i++) {
    fib[i] = min(fib[i - 1] + fib[i - 2], maxi);
  }

  while (n > 2) {
    if (k <= fib[n - 2]) {
      n -= 2;
    } else {
      k -= fib[n - 2];
      n -= 1;
    }
  }

  if (n == 1) {
    cout << "N" << endl;
  } else {
    cout << "A" << endl;
  }
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
