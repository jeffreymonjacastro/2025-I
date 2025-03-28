#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  vector<int> v(6);
  vector<int> u(6);

  for (int i = 0; i < 6; i++) {
    cin >> v[i];
  }

  u[0] = v[0] > 1 ? -v[0] + 1 : 1 - v[0];
  u[1] = v[1] > 1 ? -v[1] + 1 : 1 - v[1];
  u[2] = v[2] > 2 ? -v[2] + 2 : 2 - v[2];
  u[3] = v[3] > 2 ? -v[3] + 2 : 2 - v[3];
  u[4] = v[4] > 2 ? -v[4] + 2 : 2 - v[4];
  u[5] = v[5] > 8 ? -v[5] + 8 : 8 - v[5];

  for (int i = 0; i < 6; i++) {
    cout << u[i] << " ";
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
