#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

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
  int n;
  cin >> n;

  unordered_map<int, int> m;

  auto factor = [&](int n) -> void {
    for (int i = 2; i * i <= n; i++) {
      while (n % i == 0) {
        m[i]++;
        n /= i;
      }
    }

    if (n > 1)
      m[n]++;
  };

  factor(n);

  int cnt = 0;

  for (auto [a, b] : m) {
    cout << a << ": " << b << "\n";
    cnt += b;
  }

  cout << binPow(2, cnt) - 1 << "\n";
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
