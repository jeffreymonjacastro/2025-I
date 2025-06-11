#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const int MAX_N = 1e7;

vector<bool> is_prime(MAX_N + 1, true);
vector<int> primes;

void solve() {
  int n;
  cin >> n;

  int cnt = 0;

  for (int i = 1; i <= n; i++) {
    for (int prime : primes) {
      if (i * prime <= n) {
        cnt++;
      } else {
        break;
      }
    }
  }

  cout << cnt << "\n";
}

int main() {
  cpu();

  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i <= MAX_N; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
      if ((long long)i * i <= MAX_N) {
        for (int j = i * i; j <= MAX_N; j += i)
          is_prime[j] = false;
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
