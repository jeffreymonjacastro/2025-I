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
  ll ax, ay, bx, by, cx, cy;
  cin >> ax >> ay >> bx >> by >> cx >> cy;

  ll cross = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);

  if (cross == 0) {
    cout << "No\n";
    return;
  }

  ll dist1 = (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
  ll dist2 = (cx - bx) * (cx - bx) + (cy - by) * (cy - by);

  if (dist1 == dist2) {
    cout << "Yes\n";
  } else {
    cout << "No\n";
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
