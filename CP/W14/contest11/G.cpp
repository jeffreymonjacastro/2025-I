#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

vector<int> manacher_odd(string s) {
  int n = s.size();
  s = "$" + s + "^";
  vector<int> p(n + 2);
  int l = 0, r = 1;
  for (int i = 1; i <= n; i++) {
    p[i] = min(r - i, p[l + (r - i)]);
    while (s[i - p[i]] == s[i + p[i]]) {
      p[i]++;
    }
    if (i + p[i] > r) {
      l = i - p[i], r = i + p[i];
    }
  }
  return vector<int>(begin(p) + 1, end(p) - 1);
}

void solve(char c, string s) {
  int val, begin = -1, end = -1;
  string ss;

  ss = s;
  val = c - 'a';

  for (int i = 0; i < s.size(); i++) {
    ss[i] = ((s[i] - 'a' - val + 26) % 26) + 'a';
  }

  vector<int> p = manacher_odd(ss);

  int r = 0;
  int center = 0;

  for (int i = 0; i < p.size(); i++) {
    if (p[i] > r) {
      r = p[i];
      center = i;
    }
  }

  if (r > 1) {
    begin = center - r + 1;
    end = center + r - 1;

    cout << begin << " " << end << '\n';
    cout << ss.substr(begin, end - begin + 1) << '\n';
  } else {
    cout << "No solution!\n";
  }
}

int main() {
  cpu();
  int t;
  char c;
  string s;
  while (cin >> c >> s)
    solve(c, s);
  return 0;
}
