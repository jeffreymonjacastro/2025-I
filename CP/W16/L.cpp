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
  int n, k;
  cin >> n >> k;

  vector<int> numbers(n + 1), colors(n + 1);
  vector<int> position(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> numbers[i] >> colors[i];
    position[numbers[i]] = i;
  }

  for (int num = 1; num <= n; num++) {
    int currentPos = position[num];
    int targetPos = num;

    if (colors[currentPos] != colors[targetPos]) {
      cout << "N\n";
      return;
    }
  }

  cout << "Y\n";
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