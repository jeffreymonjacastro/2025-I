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
  for (int i = 0; i < 3; i++) {

    string s;
    cin >> s;
    vector<char> v;
    bool a, b, c;
    a = b = c = false;

    for (int i = 0; i < s.size(); i++) {
      if (s[i] != '?') {
        v.push_back(s[i]);
      }
    }

    for (char ch : v) {
      if (ch == 'A')
        a = true;
      else if (ch == 'B')
        b = true;
      else if (ch == 'C')
        c = true;
    }

    if (!a)
      cout << 'A' << endl;
    else if (!b)
      cout << 'B' << endl;
    else if (!c)
      cout << 'C' << endl;
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
