#include <algorithm>
#include <cmath>
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

void generar(vector<string> &board, vector<int> &y, int i, int n,
             vector<bool> &col, vector<bool> &dia1, vector<bool> &dia2,
             int &conta) {
  if (i == n) {
    conta++;
  } else {
    for (int j = 0; j < n; j++) {
      if (col[j] || dia1[i + j] || dia2[i + (n - 1) - j] ||
          (board[i][j] == '*'))
        continue;
      y.push_back(j);
      col[j] = dia1[i + j] = dia2[i + (n - 1) - j] = true;
      generar(board, y, i + 1, n, col, dia1, dia2, conta);
      col[j] = dia1[i + j] = dia2[i + (n - 1) - j] = false;
      y.pop_back();
    }
  }
}

void solve() {
  int n = 8;
  vector<string> board(n);
  for (int i = 0; i < n; i++) {
    cin >> board[i];
  }

  vector<int> y;
  vector<bool> col(n, false);
  vector<bool> dia1(2 * n + 1, false);
  vector<bool> dia2(2 * n - 1, false);

  int conta = 0;
  generar(board, y, 0, n, col, dia1, dia2, conta);
  cout << conta << endl;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
