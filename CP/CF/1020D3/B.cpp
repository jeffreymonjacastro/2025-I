#include <iostream>
using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, x;
  cin >> n >> x;

  if (n == 1) {
    cout << "0" << endl;
    return;
  }

  for (int i = 0; i < n; i++) {
    if (i != x)
      cout << i << " ";
  }

  if (x < n) {
    cout << x << endl;
  } else {
    cout << endl;
  }
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
