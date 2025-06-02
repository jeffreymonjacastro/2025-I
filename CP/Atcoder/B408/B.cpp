#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n;
  cin >> n;
  vector<int> v(n);
  set<int> s;

  for (int i = 0; i < n; i++) {
    cin >> v[i];
    s.insert(v[i]);
  }

  cout << s.size() << endl;

  for (const auto &elem : s) {
    cout << elem << " ";
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
