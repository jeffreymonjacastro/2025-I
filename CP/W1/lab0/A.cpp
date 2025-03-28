#include <iostream>

using namespace std;

void solve() {
  int n, sum = 0;
  cin >> n;

  while (n > 0) {
    sum += n % 10;
    n /= 10;
  }

  cout << sum << endl;
}

int main() {
  int t;
  cin >> t;

  while (t--) {
    solve();
  }

  return 0;
}
