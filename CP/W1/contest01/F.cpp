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
  string str1, str2;
  cin >> str1 >> str2;

  int h1, m1, s1, h2, m2, s2, h3, m3, s3;
  string h, m, s;

  h1 = stoi(str1.substr(0, 2));
  m1 = stoi(str1.substr(3, 5));
  s1 = stoi(str1.substr(6, 8));
  h2 = stoi(str2.substr(0, 2));
  m2 = stoi(str2.substr(3, 5));
  s2 = stoi(str2.substr(6, 8));

  if (s1 == s2 && m1 == m2 && h1 == h2) {
    cout << "24:00:00";
    return;
  }

  if (s2 >= s1) {
    s3 = s2 - s1;
  } else {
    s3 = s2 + (60 - s1);
    m2--;
  }

  if (m2 >= m1) {
    m3 = m2 - m1;
  } else {
    m3 = m2 + (60 - m1);
    h2--;
  }

  if (h2 >= h1) {
    h3 = h2 - h1;
  } else {
    h3 = h2 + (24 - h1);
  }

    s = to_string(s3).length() == 2 ? to_string(s3) : "0" + to_string(s3);
  m = to_string(m3).length() == 2 ? to_string(m3) : "0" + to_string(m3);
  h = to_string(h3).length() == 2 ? to_string(h3) : "0" + to_string(h3);

  cout << h << ":" << m << ":" << s << endl;
}

int main() {
  cpu();
  int t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
