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
  int n;
  cin >> n;
  string s;
  cin >> s;

  vector<bool> v(26);

  for (int i = 0; i < n; i++) {
    int pos = tolower(s[i]) - 'a';
    v[pos] = true;
  }

  for (int i = 0; i < 26; i++) {
    if (!v[i]) {
      cout << "NO";
      return;
    }
  }

  cout << "YES";
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
