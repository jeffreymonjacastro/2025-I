#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const int MAX_N = 1e6;
// max_div[i] contains the largest prime that goes into i
int max_div[MAX_N + 1];

void solve() {
  int x;
  cin >> x;

  int div_num = 1;
  while (x != 1) {
    /*
     * get the largest prime that can divide x and see
     * how many times it goes into x (stored in count)
     */
    int prime = max_div[x];
    int count = 0;
    while (x % prime == 0) {
      count++;
      x /= prime;
    }
    div_num *= count + 1;
  }
  cout << div_num << '\n';
}

int main() {
  cpu();

  for (int i = 2; i <= MAX_N; i++) {
    if (max_div[i] == 0) {
      for (int j = i; j <= MAX_N; j += i) {
        max_div[j] = i;
      }
    }
  }

  int t;
  t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
