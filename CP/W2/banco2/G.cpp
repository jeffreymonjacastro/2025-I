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
  string s;
  cin >> s;

  map<char, int> m;

  for (char c : s) {
    m[c]++;
  }

  if (s.size() % 2 == 0) {
    for (auto p : m) {
      if (p.second % 2 == 1) {
        cout << "NO SOLUTION";
        return;
      }
    }

    int middle;
    for (auto it = m.begin(); it != m.end(); it++) {
      middle = it->second / 2;

      for (int i = 0; i < middle; i++) {
        cout << it->first;
        it->second--;
      }
    }

    for (auto it = m.rbegin(); it != m.rend(); it++) {
      while (it->second != 0) {
        cout << it->first;
        it->second--;
      }
    }
  } else {
    int odds = 0;
    char odd_char;

    for (auto p : m) {
      if (p.second % 2 == 1) {
        odds++;
        odd_char = p.first;
      }
    }

    if (odds > 1) {
      cout << "NO SOLUTION";
      return;
    }

    auto odd_it = m.find(odd_char);

    int middle;
    for (auto it = m.begin(); it != m.end(); it++) {
      if (it == odd_it)
        continue;

      middle = it->second / 2;

      for (int i = 0; i < middle; i++) {
        cout << it->first;
        it->second--;
      }
    }

    for (int i = 0; i < odd_it->second; i++) {
      cout << odd_it->first;
    }

    odd_it->second = 0;

    for (auto it = m.rbegin(); it != m.rend(); it++) {
      while (it->second != 0) {
        cout << it->first;
        it->second--;
      }
    }
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
