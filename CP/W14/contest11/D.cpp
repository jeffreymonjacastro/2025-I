#include <functional>
#include <iostream>
#include <vector>

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
  ll n, m;
  cin >> n >> m;
  vector<int> a(n), b(m);

  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }

  vector<int> pi(m, 0);

  int j = 0, ans = -1;
  for (int i = 1; i < m; i++) {
    while (j > 0 && b[i] != b[j])
      j = pi[j - 1];
    if (b[i] == b[j])
      ++j;
    pi[i] = j;
  }

  j = 0;
  for (int i = 0; i < n; i++) {
    while (j > 0 && a[i] != b[j])
      j = pi[j - 1];
    if (a[i] == b[j])
      ++j;
    if (j == m) {
      ans = i - m + 2;
      break;
    }
  }
  cout << ans << '\n';
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
