#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  ll x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
  cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5 >> x6 >> y6;

  ll min_x = min(x4, x6);
  ll max_x = max(x3, x5);
  ll min_y = min(y4, y6);
  ll max_y = max(y3, y5);

  ll lx = max(0LL, min(x2, min_x) - max(x1, max_x));
  ll ly = max(0LL, min(y2, min_y) - max(y1, max_y));

  ll inter_black_black = lx * ly;

  lx = max(0LL, min(x2, x4) - max(x1, x3));
  ly = max(0LL, min(y2, y4) - max(y1, y3));

  ll inter_black_white1 = lx * ly;

  lx = max(0LL, min(x2, x6) - max(x1, x5));
  ly = max(0LL, min(y2, y6) - max(y1, y5));

  ll inter_black_white2 = lx * ly;

  ll area_white = (x2 - x1) * (y2 - y1);

  ll res =
      area_white - inter_black_white1 - inter_black_white2 + inter_black_black;

  if (res > 0) {
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
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
