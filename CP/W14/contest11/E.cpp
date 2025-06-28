#include <iostream>

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
  string l, s;
  int cnt, i;
  bool flag;
  while (true) {
    cin >> l;
    if (l == "#")
      break;
    cin >> s;
    cnt = 0, i = 0;
    while (i + s.size() <= l.size()) {
      flag = true;
      for (int j = 0; j < s.size(); j++) {
        if (l[i + j] != s[j]) {
          flag = false;
          break;
        }
      }
      if (flag) {
        cnt++;
        i += s.size();
      } else
        i++;
    }
    cout << cnt << '\n';
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
