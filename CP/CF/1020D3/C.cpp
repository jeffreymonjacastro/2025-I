#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int n, k, x = -1;
  cin >> n >> k;
  vector<int> a(n);
  vector<int> b(n);

  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }

  for (int i = 0; i < n; i++) {
    if (b[i] == -1)
      continue;

    int curr = a[i] + b[i];

    if (x == -1) {
      x = curr;
    } else if (x != curr) {
      cout << "0" << endl;
      return;
    }
  }

  if (x != -1) {
    for (int i = 0; i < n; i++) {
      int val = x - a[i];

      if (val < 0 || val > k) {
        cout << "0" << endl;
        return;
      }
    }

    cout << "1" << endl;
    return;
  }

  int max_x = *(max_element(a.begin(), a.end()));
  int min_x = *(min_element(a.begin(), a.end()));

  cout << (k + 1) - (max_x - min_x) << endl;
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
