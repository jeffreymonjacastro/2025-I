#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int a, b, c, d;
  cin >> a >> b >> c >> d;

  int minGelly = min(a, c);
  int minFlower = min(b, d);

  if (minGelly > minFlower) {
    cout << "Gellyfish" << endl;
  } else if (minFlower > minGelly) {
    cout << "Flower" << endl;
  } else {
    cout << "Gellyfish" << endl;
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
