#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 998244353;

ll binPow(ll a, ll b) {
  a %= mod;
  ll result = 1;
  while (b > 0) {
    if (b & 1LL)
      result = (result * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }
  return result;
}

void solve() {
  int n, m;
  cin >> n >> m;
  
  vector<vector<int>> adj(n + 1);
  
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  
  vector<int> color(n + 1, -1);
  ll result = 1;
  
  for (int i = 1; i <= n; i++) {
    if (color[i] == -1) {
      queue<int> q;
      q.push(i);
      color[i] = 0;
      bool is_bipartite = true;
      int count0 = 0, count1 = 0;
      
      while (!q.empty() && is_bipartite) {
        int u = q.front();
        q.pop();
        
        if (color[u] == 0) count0++;
        else count1++;
        
        for (int v : adj[u]) {
          if (color[v] == -1) {
            color[v] = 1 - color[u];
            q.push(v);
          } else if (color[v] == color[u]) {
            is_bipartite = false;
            break;
          }
        }
      }
      
      if (!is_bipartite) {
        cout << 0 << "\n";
        return;
      }
      
      ll ways = (binPow(2, count0) + binPow(2, count1)) % mod;
      result = (result * ways) % mod;
    }
  }
  
  cout << result << "\n";
}

int main() {
  cpu();
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}