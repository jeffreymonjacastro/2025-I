#include <bits/stdc++.h>

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

  vector<pair<int, int>> v(n);

  int a;
  for (int i = 0; i < n; i++) {
    cin >> a;
    v[i].first = a;
    v[i].second = i + 1;
  }

  sort(v.begin(), v.end());

  cout << v.back().second << endl;
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
