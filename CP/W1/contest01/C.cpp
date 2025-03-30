#include <cmath>
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
  int n, x, sum = 0;
  cin >> n;
  unordered_map<int, int> m;

  for (int i = 0; i < n; i++) {
    cin >> x;
    m[x]++;
  }

  for (pair<int, int> p : m) {
    sum += ceil(p.second / (double)(p.first + 1)) * (p.first + 1);
  }

  cout << sum;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
