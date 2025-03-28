
#include <iostream>
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
  int n, min = 1e9;
  cin >> n;

  vector<int> v(n);
  unordered_map<int, vector<int>> m(3);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
    m[v[i] - 1].push_back(i + 1);
  }

  for (int i = 0; i < 3; i++) {
    if (m[i].size() < min)
      min = m[i].size();
  }

  vector<vector<int>> triplas;
  vector<int> temp(3);

  for (int j = 0; j < min; j++) {
    for (int i = 0; i < 3; i++) {
      if (m[i].empty())
        break;
      temp[i] = m[i].back();
      m[i].pop_back();
    }

    triplas.push_back(temp);
  }

  cout << triplas.size() << endl;

  for (int i = 0; i < triplas.size(); i++) {
    for (int j = 0; j < 3; j++) {
      cout << triplas[i][j] << " ";
    }
    cout << endl;
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
