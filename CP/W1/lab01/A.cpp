#include <algorithm>
#include <iostream>
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

  vector<int> v(n - 1);
  vector<bool> u(n + 1);

  for (int i = 0; i < n - 1; i++) {
    cin >> v[i];
    u[v[i]] = true;
  }

  for (int i = 1; i < n + 1; i++) {
    if (!u[i]) {
      cout << i;
      return;
    }
  }
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
