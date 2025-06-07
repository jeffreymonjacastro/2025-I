#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
#define f first
#define s second

const int mod = 1e9 + 7;

ll binPow(ll a, ll b) {
  a %= mod;
  ll result = 1;
  while (b > 0) {
    if (b & 1LL)
      result = (result * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }
  return result;
}

void solve() {
  int n;
  cin >> n;

  unordered_map<int, int> m;

  auto factor = [&](int n) -> void {
    for (int i = 2; i * i <= n; i++) {
      while (n % i == 0) {
        m[i]++;
        n /= i;
      }
    }

    if (n > 1)
      m[n]++;
  };

  factor(n);

  int total = 0;
  for (auto [p, count] : m) {
    total += count;
  }

  bool flag = false;

  if (total >= 3) {
    if (m.size() >= 2) {
      flag = true;
    } else if (m.size() == 1 && total >= 6) {
      flag = true;
    }
  }

  if (flag) {
    ll a = 1, b = 1, c = 1;

    if (m.size() == 1) {
      ll p = m.begin()->f;
      a = p;
      b = p * p;
      c = n / (a * b);
    } else if (m.size() == 2) {
      auto it = m.begin();
      ll p1 = it->f;
      it++;
      ll p2 = it->f;

      a = p1;
      b = p2;
      c = n / (a * b);
    } else {
      auto it = m.begin();
      a = it->f;
      it++;
      b = it->f;
      it++;
      c = it->f;

      ll r = n / (a * b * c);
      c *= r;
    }

    if (a == b || b == c || a == c) {
      cout << "NO\n";
      return;
    }

    cout << "YES\n";
    cout << a << " " << b << " " << c << "\n";

  } else {
    cout << "NO\n";
  }
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