#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define pii pair<int, int>
#define pll pair<ll, ll>
#define f first
#define s second

const int mod = 1e9 + 7;

void floydWarshall(vector<vector<int>> &dist) {
  int n = dist.size();
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }
}

void solve() {
  string s, t;
  cin >> s >> t;

  if (s.size() != t.size()) {
    cout << -1 << endl;
    return;
  }

  int n;
  cin >> n;

  vector<vector<int>> dist(26, vector<int>(26, INT_MAX));

  char a, b;
  int w;

  for (int i = 0; i < n; i++) {
    cin >> a >> b >> w;
    dist[a - 'a'][b - 'a'] = min(dist[a - 'a'][b - 'a'], w);
  }

  for (int i = 0; i < 26; i++) {
    dist[i][i] = 0;
  }

  floydWarshall(dist);

  ll total_cost = 0;
  string res;

  for (int i = 0; i < s.size(); i++) {
    if (s[i] == t[i]) {
      res += s[i];
      continue;
    }

    int id_s = s[i] - 'a';
    int id_t = t[i] - 'a';

    int case1 = dist[id_s][id_t];
    int case2 = dist[id_t][id_s];
    int case3 = INT_MAX;
    char new_char;

    for (int j = 0; j < 26; j++) {
      if (dist[id_s][j] != INT_MAX && dist[id_t][j] != INT_MAX) {
        if (case3 > dist[id_s][j] + dist[id_t][j]) {
          case3 = dist[id_s][j] + dist[id_t][j];
          new_char = 'a' + j;
        }
      }
    }

    if (case1 == INT_MAX && case2 == INT_MAX && case3 == INT_MAX) {
      cout << -1 << endl;
      return;
    }

    if (case1 <= case2 && case1 <= case3) {
      total_cost += case1;
      res += t[i];
    } else if (case2 <= case1 && case2 <= case3) {
      total_cost += case2;
      res += s[i];
    } else {
      total_cost += case3;
      res += new_char;
    }
  }

  cout << total_cost << endl;
  cout << res << endl;
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
