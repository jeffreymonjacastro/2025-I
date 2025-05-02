#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

int binarySearch(vector<int> &v, int l, int r, int x) {
  if (l > r)
    return -1;

  int m = (l + r) / 2;
  if (x == v[m])
    return m;
  else if (x < v[m])
    return binarySearch(v, l, m - 1, x);
  else
    return binarySearch(v, m + 1, r, x);
}

void solve() {
  int n, x;
  cin >> n;

  vector<int> v(n);
  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  cin >> x;

  cout << binarySearch(v, 0, v.size() - 1, x);
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
