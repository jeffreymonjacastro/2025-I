#include <iostream>
#include <vector>

#define mod 1000000007

using namespace std;

int num_of_triangularions(int n) {
  vector<int> dp(n + 1, 0);
  dp[2] = 1;
  dp[3] = 1;

  for (int i = 4; i <= n; i++) {
    for (int j = 2; j < i; j++) {
      dp[i] += (dp[j] * dp[i - j + 1]) % mod;
      dp[i] %= mod;
    }
  }

  return dp[n];
}

int main() {

  for (int i = 0; i < 100; i++) {
    cout << i << ": " << num_of_triangularions(i) << endl;
  }

  return 0;
}