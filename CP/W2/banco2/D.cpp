#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, arrows = 0;
  cin >> n;
  vector<int> v(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  vector<int> u(1000001, 0);

  for (int i = 0; i < n; i++) {
    if (u[v[i]] > 0) {
      u[v[i]]--;
      u[v[i] - 1]++;
    } else {
      arrows++;
      u[v[i] - 1]++;
    }
  }

  cout << arrows;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
