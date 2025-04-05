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

string binToString(ll n) {
  string res;
  while (n > 0) {
    res += to_string(n % 2);
    n = n / 2;
  }
  return res;
}

void solve() {
  ll n, sol = 0;
  cin >> n;

  string bin = binToString(n);

  int l = bin.size();

  for (int i = 0; i < l; i++) {
    sol += (bin[i] - '0') * (1LL << (l - i - 1));
  }

  cout << sol;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
