#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int a, b, c, count = 0;
  cin >> a >> b;

  if (a == b) {
    cout << count << endl;
    return;
  }

  c = a - b;

  count = abs(c / 10);

  if (abs(c % 10)) {
    count++;
  }

  cout << count << endl;
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
