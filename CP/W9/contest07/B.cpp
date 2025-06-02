#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  string s;
  cin >> s;

  vector<set<int>> adj(26);

  for (int i = 0; i < s.size() - 1; i++) {
    int u = s[i] - 'a';
    int v = s[i + 1] - 'a';
    adj[u].insert(v);
    adj[v].insert(u);
  }

  for (int i = 0; i < 26; i++) {
    if (adj[i].size() > 2) {
      cout << "NO\n";
      return;
    }
  }

  vector<int> ans;
  vector<bool> visited(26, false);

  for (int i = 0; i < 26; i++) {
    if (!visited[i] && adj[i].size() <= 1) {
      visited[i] = true;
      ans.push_back(i);

      if (adj[i].size() == 1) {
        int prev = i;
        int curr = *adj[i].begin();

        while (adj[curr].size() == 2) {
          visited[curr] = true;
          ans.push_back(curr);

          int a = *adj[curr].begin();
          int b = *next(adj[curr].begin());

          int temp = (a == prev) ? b : a;
          prev = curr;
          curr = temp;
        }

        visited[curr] = true;
        ans.push_back(curr);
      }
    }
  }

  if (ans.size() < 26) {
    cout << "NO\n";
    return;
  }

  cout << "YES\n";
  for (int i = 0; i < ans.size(); i++) {
    cout << char(ans[i] + 'a');
  }
  cout << "\n";
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
