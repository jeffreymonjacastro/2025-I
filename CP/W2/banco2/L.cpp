#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  ll n, swaps = 0;
  cin >> n;
  vector<ll> v(n);
  vector<bool> u(n, false);

  for (ll i = 0; i < n; i++) {
    cin >> v[i];
  }

  for (ll i = 0; i < n - 1; i++) {
    if (v[i] == i + 1) {
      u[i] = true;
    }

    if (u[i]) {
      continue;
    }

    swap(v[i], v[i + 1]);
    swaps++;

    if (v[i] != i + 1) {
      i--;
    }
  }

  cout << swaps << endl;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
