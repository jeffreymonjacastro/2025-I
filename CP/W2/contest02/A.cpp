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
  int n;
  ll bell = 0;
  cin >> n;
  vector<int> v(n);
  string s;

  for (int i = 0; i < n; i++) {
    cin >> s;
    if (s == "Z") {
      v[i] = 1;
    } else if (s == "O") {
      v[i] = 0;
    }
  }

  for (int i = 0; i < n; i++) {
    v[i] = 1 ^ v[i];
  }

  for (int i = 0; i < n; i++) {
    bell += v[i] * (1LL << (n - i - 1));
  }

  cout << bell << endl;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
