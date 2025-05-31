#include <bits/stdc++.h>
using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

int dx[] = {1, 0, 0, -1};
int dy[] = {0, -1, 1, 0};
char dir_char[] = {'D', 'L', 'R', 'U'};
string result;

void bfs(vector<string> &maze, int n, int m, int start_x, int start_y, vector<vector<int>> &dist) {
  queue<pair<int, int>> q;
  q.push({start_x, start_y});
  dist[start_x][start_y] = 0;

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();

    for (int i = 0; i < 4; i++) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
          maze[nx][ny] != '*' && dist[nx][ny] == -1) {

        dist[nx][ny] = dist[x][y] + 1;
        q.push({nx, ny});
      }
    }
  }
}

bool dfs(vector<string> &maze, vector<vector<int>> &dist, int x, int y, int steps_left, int n, int m, int start_x, int start_y, string &path) {
  if (steps_left == 0) {
    if (x == start_x && y == start_y) {
      result = path;
      return true;
    }
    return false;
  }

  if (dist[x][y] == -1 || steps_left < dist[x][y] || (steps_left - dist[x][y]) % 2 != 0) {
    return false;
  }

  for (int i = 0; i < 4; i++) {
    int nx = x + dx[i];
    int ny = y + dy[i];

    if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
        maze[nx][ny] != '*') {

      path += dir_char[i];

      if (dfs(maze, dist, nx, ny, steps_left - 1, n, m,
              start_x, start_y, path)) {
        return true;
      }

      path.pop_back();
    }
  }

  return false;
}

void solve() {
  int n, m, k;
  cin >> n >> m >> k;
  int start_x, start_y;

  vector<string> maze(n);

  string fil;
  for (int i = 0; i < n; i++) {
    cin >> fil;
    maze[i] = fil;

    for (int j = 0; j < m; j++) {
      if (fil[j] == 'X') {
        start_x = i;
        start_y = j;
      }
    }
  }

  vector<vector<int>> dist(n, vector<int>(m, -1));

  bfs(maze, n, m, start_x, start_y, dist);

  if (dfs(maze, dist, start_x, start_y, k, n, m, start_x, start_y, result)) {
    cout << result << "\n";
  } else {
    cout << "IMPOSSIBLE\n";
  }
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