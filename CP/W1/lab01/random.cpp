#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve(int t) {
  vector<int> v(3);
  cin >> v[0] >> v[1] >> v[2];
  int max_num = max(v[0], max(v[1], v[2]));

  auto it = find(v.begin(), v.end(), max_num);
  if (it != v.end()) {
    v.erase(it);
  }

  int min_num = min(v[0], min(v[1], v[2]));

  it = find(v.begin(), v.end(), min_num);
  if (it != v.end()) {
    v.erase(it);
  }

  for (int i : v) {
    cout << "Case " << t << ": " << i << endl;
  }
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  for (int i = 1; i < t + 1; i++)
    solve(i);
  return 0;
}
