#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void generar(vector<vector<int>> &numbers, vector<int> &v, int it, int row,
             int column, int max_iter, int n, int &min, int &closest) {
  if (row >= 4 || column >= 3 || numbers[row][column] == -1)
    return;

  if (it == max_iter) {
    int number = 0;
    for (int dig : v) {
      number = number * 10 + dig;
    }

    int diff = abs(number - n);
    if (diff < min) {
      min = diff;
      closest = number;
    }

    return;
  }

  v.push_back(numbers[row][column]);
  generar(numbers, v, it + 1, row, column, max_iter, n, min, closest);
  v.pop_back();

  generar(numbers, v, it, row + 1, column, max_iter, n, min, closest);

  generar(numbers, v, it, row, column + 1, max_iter, n, min, closest);
}

void solve() {
  int n;
  cin >> n;
  vector<vector<int>> numbers = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {-1, 0, -1}};
  vector<int> v;
  int max_iter = floor(log10(n)) + 1;
  int min = 1e9;
  int closest = -1;

  generar(numbers, v, 0, 0, 0, max_iter, n, min, closest);

  cout << closest << endl;
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
