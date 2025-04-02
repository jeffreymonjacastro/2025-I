#include <iostream>
#include <map>
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
  int n;

  while (cin >> n) {
    vector<int> v(n);
    set<int> s;

    for (int i = 0; i < n; i++) {
      cin >> v[i];
    }

    for (int i = 1; i < n; i++) {
      int diff = abs(v[i] - v[i - 1]);
      s.insert(diff);
    }

    bool isJolly = true;

    for (int i = 1; i < n; i++) {
      if (s.find(i) == s.end()) {
        isJolly = false;
      }
    }

    isJolly ? cout << "Jolly" << endl : cout << "Not jolly" << endl;
  }
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  solve();
  return 0;
}
