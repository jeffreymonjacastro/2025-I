#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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
  vector<ll> x(n, 0), y(n, 0);

  map<pair<ll, ll>, ll> m;

  for (int i = 0; i < n; i++)
    cin >> x[i] >> y[i];

  ll ans = 0;

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      int a = x[i] + x[j];
      int b = y[i] + y[j];
      m[{a, b}]++;
    }
  }

  for (auto const &k : m) {
    ans = ans + (k.second * k.second - k.second) / 2;
  }

  cout << ans << endl;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
