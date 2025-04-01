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
  int n;
  cin >> n;

  vector<int> v(n);
  vector<int> res;

  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  res.push_back(v[0]);
  int pointer = 0;

  for (int i = 1; i < n; i++) {
    if (v[i] > res[pointer]) {
      res.push_back(v[i]);
      pointer++;
    }
  }

  cout << res.size() << endl;

  for (int i : res) {
    cout << i << " ";
  }
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
