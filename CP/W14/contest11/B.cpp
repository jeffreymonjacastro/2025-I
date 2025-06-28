#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const ll N = 2e5;
int n, m;
int a[2 * N + 2];
int pi[2 * N + 2];

ll binPow(ll a, ll b) {
  a %= mod;
  ll result = 1;
  while (b > 0) {
    if (b & 1LL)
      result = (result * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }
  return result;
}

void solve() {
  a[m] = 1e9 + 1;
  int pv{};
  cin >> pv;
  for (int i = 1; i < n; ++i) {
    int x;
    cin >> x;
    a[m + i] = x - pv;
    pv = x;
  }
  cin >> pv;
  for (int i = 1; i < m; ++i) {
    int x;
    cin >> x;
    a[i] = x - pv;
    pv = x;
  }

  if (m == 1) {
    cout << n << '\n';
    return;
  }

  int ans{};
  pi[1] = 0;

  for (int i = 2; i < m + n; ++i) {
    int j{pi[i - 1]};
    while (j && a[j + 1] != a[i])
      j = pi[j];
    if (a[j + 1] == a[i])
      ++j;
    pi[i] = j;
    if (j == m - 1)
      ++ans;
  }

  cout << ans << '\n';

  return;
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (cin >> n >> m)
    solve();
  return 0;
}
