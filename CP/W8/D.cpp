#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  int N, K;
  cin >> N >> K;

  vector<int> n(N);
  vector<int> c(N);

  for (int i = 0; i < N; i++) {
    cin >> n[i] >> c[i];
  }

  for (int i = 0; i < N; i++) {
    if (c[i] != c[n[i]-1]){
      cout << "N" << endl;
      return;
    }
  }

  cout << "Y" << endl;

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
