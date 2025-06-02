#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, s;
  cin >> n >> s;

  vector<int> v(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  if (n == 1 && v[0] > s) {
    cout << "No" << endl;
    return;
  }

  if (v[0] > s) {
    cout << "No" << endl;
    return;
  }

  for (int i = 1; i < n; i++) {
    if (v[i] - v[i - 1] > s) {
      cout << "No" << endl;
      return;
    }
  }

  cout << "Yes" << endl;
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
