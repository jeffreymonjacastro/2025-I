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
  int n;
  cin >> n;
  vector<int> v(n);
  vector<int> u(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
    u[i] = i + 1;
  }

  if (n == 1) {
    cout << "-1" << endl;
    return;
  }

  for (int i = 0; i < n - 1; i++) {
    if (u[i] == v[i]) {
      swap(u[i], u[i + 1]);
    }
  }

  if (u[n - 1] == v[n - 1]) {
    swap(u[n - 2], u[n - 1]);
  }

  for (int i = 0; i < n; i++) {
    cout << u[i] << " ";
  }
  cout << "\n";
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
