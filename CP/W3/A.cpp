#include <cmath>
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
  int A, B, C;
  cin >> A >> B >> C;

  for (int x = -99; x < 100; x++) {
    for (int y = -99; y < 100; y++) {
      for (int z = -99; z < 100; z++) {
        if (x == y || x == z || y == z) {
          continue;
        }

        if (((x + y + z) == A) && ((x * y * z) == B) &&
            ((x * x + y * y + z * z) == C)) {
          cout << x << " " << y << " " << z << endl;
          return;
        }
      }
    }
  }

  cout << "No solution." << endl;
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
