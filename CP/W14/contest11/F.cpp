#include <iostream>
#include <string>
#include <vector>

using namespace std;
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const int N = 1e6;
char s[N + 1];
int z[N];

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

vector<int> manacher_odd(string s) {
  int n = s.size();
  s = "$" + s + "^";
  vector<int> p(n + 2, 0);
  int l = 0, r = 1;
  for (int i = 1; i <= n; i++) {
    p[i] = min(r - i, p[l + (r - i)]);
    while (s[i - p[i]] == s[i + p[i]]) {
      p[i]++;
    }
    if (i + p[i] > r) {
      l = i - p[i];
      r = i + p[i];
    }
  }
  vector<int> res;
  for (int i = 1; i <= n; ++i)
    res.push_back(p[i]);
  return res;
}

vector<int> manacher(string s) {
  string t = "";
  for (size_t i = 0; i < s.size(); ++i) {
    t += "#";
    t += s[i];
  }
  t += "#";
  vector<int> odd = manacher_odd(t);
  vector<int> res;
  for (size_t i = 1; i + 1 < odd.size(); ++i)
    res.push_back(odd[i]);
  return res;
}

void solve() {
  string s;
  int c = 1;
  while (true) {
    cin >> s;
    if (s == "END")
      break;
    vector<int> res = manacher(s);
    int ans = 1;
    for (size_t i = 0; i < res.size(); ++i) {
      if (res[i] > ans)
        ans = res[i];
    }
    cout << "Case " << c << ": " << ans - 1 << '\n';
    c++;
  }
}

int main() {
  int t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
