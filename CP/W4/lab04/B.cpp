#include <bits/stdc++.h>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);

// int binaryExponentiation(int a, int b) {
//   if (b == 0)
//     return 1;

//   if (b % 2 == 0) {
//     int res = binaryExponentiation(a, b / 2);
//     return (res * res) % 10;
//   } else {
//     return a * binaryExponentiation(a, b - 1) % 10;
//   }
// }

int binPow(int a, int b) {
  a %= 10;
  int result = 1;

  while (b > 0) {
    if (b & 1)
      result = (result * a) % 10;
    a = (a * a) % 10;
    b >>= 1;
  }

  return result;
}

void solve() {
  int a, b;
  cin >> a >> b;

  if (a == 0) {
    cout << 0 << endl;
    return;
  }

  cout << binPow(a, b) << endl;
}

int main() {
  cpu();
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
