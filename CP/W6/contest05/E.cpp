#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;
const lld tol = 1e-9;

void solve() {
  int n, t;
  cin >> n >> t;

  vector<lld> d(n);
  vector<lld> s(n);

  for (int i = 0; i < n; i++) {
    cin >> d[i] >> s[i];
  }

  int min_s = *min_element(s.begin(), s.end());

  lld l = -min_s + 1e-9;
  lld r = 1e9;

  while (r - l > tol) {
    lld c = (l + r) / 2;
    lld time = 0;

    for (int i = 0; i < n; i++) {
      time += d[i] / (s[i] + c);
    }

    if (time > t) {
      l = c;
    } else {
      r = c;
    }
  }

  cout << fixed << setprecision(9) << l;
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
