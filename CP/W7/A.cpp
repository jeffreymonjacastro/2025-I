#include <bits/stdc++.h>

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

  vector<bool> visited(26, false);
  vector<bool> completed(26, false);
  vector<int> pos(26, -1);

  for (int i = 0; i < n; i++) {
    int id = s[i] - 'A';

    if (!visited[id]) {
      visited[id] = true;
      pos[id] = i;
    }

    if (!completed[id] && i - pos[id] >= id) {
      completed[id] = true;

      cout << i << ": " << char('A' + id) << endl;
    }
  }

  int count = 0;

  for (int i = 0; i < 26; i++) {
    if (completed[i]) {
      count++;
    }
  }

  cout << count << "\n";
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
