#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, t;
  cin >> n >> t;

  vector<int> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }

  // BFS para encontrar el mínimo número de pasos
  vector<int> dist(
      3601, -1); // dist[i] = mínimo número de pasos para llegar al tiempo i
  queue<int> q;

  q.push(0);
  dist[0] = 0;

  while (!q.empty()) {
    int current = q.front();
    q.pop();

    for (int i = 0; i < n; i++) {
      int next = current + b[i];

      // Clamping: el tiempo debe estar entre [0, 3600]
      if (next < 0)
        next = 0;
      if (next > 3600)
        next = 3600;

      // Si aún no hemos visitado este tiempo
      if (dist[next] == -1) {
        dist[next] = dist[current] + 1;
        q.push(next);
      }
    }
  }

  // Si podemos llegar exactamente al tiempo t
  if (dist[t] != -1) {
    cout << dist[t] << " 0\n";
    return;
  }

  // Si no, encontrar el menor tiempo > t que sea alcanzable
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

int main() {
  cpu();
  int t;
  cin >> t; // Leer número de casos de prueba
  while (t--)
    solve();
  return 0;
}