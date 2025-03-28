#include <iostream>
using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

bool calculateRounds(int x1, int x2, int y1, int y2) {
  int round_sunnet = (x1 > y1) + (x2 > y2);
  int round_slavic = (x1 < y1) + (x2 < y2);
  return round_sunnet > round_slavic;
}

void solve() {
  int a1, a2, b1, b2, game_sunnet = 0;
  cin >> a1 >> a2 >> b1 >> b2;

  // First comparison: a1, a2 vs b1, b2
  if (calculateRounds(a1, a2, b1, b2)) {
    game_sunnet++;
  }

  // Second comparison: a1, a2 vs b2, b1
  if (calculateRounds(a1, a2, b2, b1)) {
    game_sunnet++;
  }

  // Third comparison: a2, a1 vs b1, b2
  if (calculateRounds(a2, a1, b1, b2)) {
    game_sunnet++;
  }

  // Fourth comparison: a2, a1 vs b2, b1
  if (calculateRounds(a2, a1, b2, b1)) {
    game_sunnet++;
  }

  cout << game_sunnet << endl;
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
