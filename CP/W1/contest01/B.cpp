#include <iostream>
#include <sstream>
#include <string>
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
  int a, b, c;
  cin >> a >> b >> c;

  vector<int> v(10, 0);

  v[a]++;
  v[b]++;
  v[c]++;

  for (int i = 0; i < 10; i++) {
    if (v[i] == 1) {
      cout << i << endl;
      return;
    }
  }
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
