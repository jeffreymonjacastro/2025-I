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

void solve() {
  int n, count = 0;
  cin >> n;
  vector<int> v(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }

  vector<int> max_left(n), min_right(n);

  max_left[0] = -1e9;
  for (int i = 1; i < n; i++) {
    max_left[i] = max(max_left[i - 1], v[i - 1]);
  }

  min_right[n - 1] = 1e9;
  for (int i = n - 2; i >= 0; i--) {
    min_right[i] = min(min_right[i + 1], v[i + 1]);
  }

  int pivot;
  for (int i = 0; i < n; i++) {
    pivot = v[i];

    if (max_left[i] < pivot && min_right[i] > pivot) {
      count++;
    }
  }

  cout << count;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
