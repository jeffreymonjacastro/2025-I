#include <algorithm>
#include <cmath>
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
  int n, m, max = -1e9;
  cin >> n >> m;
  unordered_map<int, int> napita;
  vector<int> v;

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      napita[i + j]++;
    }
  }

  for (pair<int, int> p : napita) {
    if (p.second > max) {
      max = p.second;
      v.clear();
      v.push_back(p.first);
    } else if (p.second == max) {
      v.push_back(p.first);
    }
  }

  sort(v.begin(), v.end());

  for (int val : v) {
    cout << val << endl;
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
