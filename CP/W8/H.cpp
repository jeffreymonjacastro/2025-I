#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n; cin>>n;
  vector<ll> c(n);
  vector<string> s(n);

  for (int i = 0; i < n; i++) {
    cin >> c[i] >> s[i];
  }

  ll minA = INT_MAX;
  ll minB = INT_MAX;
  ll minC = INT_MAX;
  ll minAB = INT_MAX;
  ll minAC = INT_MAX;
  ll minBC = INT_MAX;
  ll minABC = INT_MAX;

  for (int i = 0; i < n; i++) {
    if (s[i] == "A") {
      minA = min(minA, c[i]);
    } else if (s[i] == "B") {
      minB = min(minB, c[i]);
    } else if (s[i] == "C") {
      minC = min(minC, c[i]);
    } else if (s[i] == "AB" || s[i] == "BA") {
      minAB = min(minAB, c[i]);
    } else if (s[i] == "AC" || s[i] == "CA") {
      minAC = min(minAC, c[i]);
    } else if (s[i] == "BC" || s[i] == "CB") {
      minBC = min(minBC, c[i]);
    } else if (s[i] == "ABC" || s[i] == "ACB" ||
               s[i] == "BAC" || s[i] == "BCA" || s[i] == "CAB" ||
               s[i] == "CBA") {
      minABC = min(minABC, c[i]);
    }
  }

  if (minA == INT_MAX && minAB == INT_MAX && minAC == INT_MAX && minABC == INT_MAX) {
    cout << -1 << endl;
    return;
  }

  if (minB == INT_MAX && minAB == INT_MAX && minBC == INT_MAX && minABC == INT_MAX) {
    cout << -1 << endl;
    return;
  }

  if (minC == INT_MAX && minAC == INT_MAX && minBC == INT_MAX && minABC == INT_MAX) {
    cout << -1 << endl;
    return;
  }

  ll ans = INT_MAX;
  ans = min(ans, minA + minB + minC);
  ans = min(ans, minAB + minC);
  ans = min(ans, minAC + minB);
  ans = min(ans, minBC + minA);
  ans = min(ans, minAB + minAC);
  ans = min(ans, minAB + minBC);
  ans = min(ans, minAC + minBC);
  ans = min(ans, minABC);

  cout << ans << endl;
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
