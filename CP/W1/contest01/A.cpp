#include <iostream>
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
  vector<int> v;
  string s;
  cin >> s;
  if (isdigit(s[0])) {
    stringstream ss(s);
    string digit;
    while (!ss.eof()) {
      getline(ss, digit, '+');
      v.push_back(stoi(digit));
    }
    cout << v[0] + v[1] << endl;
  } else {
    cout << "skipped" << endl;
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
