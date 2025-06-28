#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const int N = 1e6;
char s[N + 1];
int z[N];

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
  int n{(int)strlen(s)}, x{}, y{};
  for (int i = 1; i < n; ++i) {
    z[i] = y < i ? 0 : min(y - i + 1, z[i - x]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > y)
      x = i, y = i + z[i] - 1;
  }
  for (int i = n - 1; i > 0; --i) {
    if (i + z[i] == n)
      printf("%d ", z[i]);
  }
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (scanf("%s", s) != EOF)
    solve();
  return 0;
}
