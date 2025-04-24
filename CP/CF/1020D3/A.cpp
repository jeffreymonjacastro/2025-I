#include <iostream>
#include <vector>

using namespace std;

#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, count = 0;
  vector<string> bin;
  cin >> n;

  string s, a;
  cin >> s;

  for (int i = 0; i < n; i++) {
    a = s;
    if (s[i] == '1') {
      a[i] = '0';
    } else {
      a[i] = '1';
    }

    bin.push_back(a);
  }

  for (string i : bin) {
    for (char c : i) {
      if (c == '1')
        count++;
    }
  }
  cout << count << endl;
}

int main() {
  cpu();
  int t;
  t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
