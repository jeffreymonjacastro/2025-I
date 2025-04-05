#include <algorithm>
#include <cmath>
#include <deque>
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
  ll n;
  cin >> n;
  cin.ignore();

  string s;
  deque<ll> left, right;

  for (ll i = 0; i < n; i++) {
    getline(cin, s);
    ll p = s.find(" ");

    string instr = s.substr(0, p);
    ll num = stoi(s.substr(p + 1, s.size()));

    if (instr == "push_back") {
      right.push_back(num);
    } else if (instr == "push_front") {
      left.push_front(num);
    } else if (instr == "push_middle") {
      ll tot = left.size() + right.size();
      ll mid = (tot + 1) / 2;

      if (mid <= left.size()) {
        left.push_back(num);
      } else {
        right.push_front(num);
      }
    } else {
      if (num < left.size()) {
        cout << left[num] << endl;
      } else {
        cout << right[num - left.size()] << endl;
      }
    }

    while (left.size() > right.size() + 1) {
      right.push_front(left.back());
      left.pop_back();
    }
    while (right.size() > left.size()) {
      left.push_back(right.front());
      right.pop_front();
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
