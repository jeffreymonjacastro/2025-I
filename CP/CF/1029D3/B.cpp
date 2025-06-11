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

  vector<int> sol(n);

  int k = (n + 1) / 2;
  int cnt = 1;

  for (int i = 0; i < k; i++) {
    sol[i] = cnt;
    cnt+=2;
  }

  cnt = (n % 2 == 0) ? n : n - 1;

  for (int i = k; i < n; i++) {
    sol[i] = cnt;
    cnt-=2;
  }

  for (int i = 0; i < n; i++) {
    cout << sol[i] << " ";
  }

  cout << "\n";
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
