#include <bits/stdc++.h>
using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, t;
  cin >> n >> t;

  vector<int> v(n);
  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  vector<int> dist(3601, -1);
  dist[0] = 0;

  queue<int> q;
  q.push(0);

  while (!q.empty()) {
    int curr_time = q.front();
    q.pop();

    int button = dist[curr_time];

    for (int i = 0; i < n; i++) {
      int new_time = curr_time + v[i];
      new_time = max(0, min(3600, new_time));

      if (dist[new_time] == -1) {
        dist[new_time] = button + 1;
        q.push(new_time);
      }
    }
  }

  if (dist[t] != -1) {
    cout << dist[t] << " " << 0 << "\n";
    return;
  }

  for (int time = t + 1; time <= 3600; time++) {
    if (dist[time] != -1) {
      cout << dist[time] << " " << (time - t) << "\n";
      return;
    }
  }
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