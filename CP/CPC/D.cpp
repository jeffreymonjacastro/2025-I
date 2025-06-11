#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define pii pair<int, int>
#define f first
#define s second

const int mod = 1e9 + 7;

void solve() {
  int n;
  cin >> n;

  vector<pii> p(n);

  for (int i = 0; i < n; i++) {
    cin >> p[i].f >> p[i].s;
  }

  int cnt = 0;

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (p[i].f == p[j].f || p[i].s == p[j].s ||
          abs(p[i].f - p[j].f) == abs(p[i].s - p[j].s)) {
        cnt += 2;
      }
    }
  }

  cout << cnt << "\n";
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
