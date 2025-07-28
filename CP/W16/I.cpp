#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

template <typename A, typename B>
ostream &operator<<(ostream &os, const pair<A, B> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}
template <typename T_container, typename T = typename enable_if<
                                    !is_same<T_container, string>::value,
                                    typename T_container::value_type>::type>
ostream &operator<<(ostream &os, const T_container &v) {
  os << '{';
  string sep;
  for (const T &x : v)
    os << sep << x, sep = ", ";
  return os << '}';
}
void dbg_out() { cerr << endl; }
template <typename Head, typename... Tail> void dbg_out(Head H, Tail... T) {
  cerr << ' ' << H;
  dbg_out(T...);
}
#ifdef LOCAL
#define dbg(...) cerr << "(" << #_VA_ARGS_ << "):", dbg_out(_VA_ARGS_)
#else
#define dbg(...)
#endif

#define ar array
#define ll long long
#define ld long double
#define sza(x) ((int)x.size())
#define all(a) (a).begin(), (a).end()

const int MAX_N = 1e5 + 5;
const ll MOD = 10000007;
const ll INF = 1e9;
const ld EPS = 1e-9;

auto binpow = [](ll a, ll b, ll m) {
  // a %= m;
  ll res = 1;
  while (b > 0) {
    if (b & 1)
      res = res * a;
    a = a * a;
    b >>= 1;
  }
  return res;
};

void solve() {
  ll n, R;
  cin >> n >> R;
  const int MX = 5000 + 100;
  static ll low[MX], inv[MX], dp[MX][2];
  ll MOD = 1000000007;

  auto binexp = [&](ll a, ll b) {
    ll r = 1;
    while (b) {
      if (b & 1)
        r = r * a % MOD;
      a = a * a % MOD;
      b >>= 1;
    }
    return r;
  };

  for (int i = 1; i <= n + 10; i++) {
    inv[i] = binexp(i, MOD - 2);
  }

  for (int i = 1; i <= n; i++) {
    cin >> low[i];
  }
  for (int i = n - 1; i >= 1; i--) {
    low[i] = max(low[i], low[i + 1]);
  }

  memset(dp, -1, sizeof dp);
  function<ll(int, int)> f = [&](int i, int h) -> ll {
    if (i > n)
      return h ? MOD - 1 : 1;
    ll &r = dp[i][h];
    if (r != -1)
      return r;
    r = 0;
    ll N = R - low[i] + 1, c = N % MOD;
    for (int k = 1; k <= n - i + 1; k++) {
      if (k > N)
        break;
      int nh = (h + k - 1) & 1;
      r = (r + c * f(i + k, nh)) % MOD;
      c = c * ((N - k) % MOD) % MOD * inv[k + 1] % MOD;
    }
    return r;
  };

  cout << f(1, 0) << endl;
}

int main() {
  solve();
  return 0;
}