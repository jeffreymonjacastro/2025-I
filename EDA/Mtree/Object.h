#ifndef OBJECT_H
#define OBJECT_H

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class Object {
private:
  string value;

  // Distancia de Levenshtein
  static size_t levenshtein(const string &a, const string &b) {
    const size_t n = a.size();
    const size_t m = b.size();

    vector<vector<size_t>> dp(n + 1, vector<size_t>(m + 1));

    // Inicialización de bordes
    for (size_t i = 0; i <= n; ++i)
      dp[i][0] = i;
    for (size_t j = 0; j <= m; ++j)
      dp[0][j] = j;

    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 1; j <= m; ++j) {
        size_t cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
        dp[i][j] = min({
            dp[i - 1][j] + 1,       // eliminación
            dp[i][j - 1] + 1,       // inserción
            dp[i - 1][j - 1] + cost // sustitución
        });
      }
    }
    return dp[n][m];
  }

public:
  explicit Object(string s) : value(move(s)) {}

  size_t distance(const Object *other) const {
    return levenshtein(this->value, other->value);
  }

  const string &str() const noexcept {
    return this->value;
  }
};

#endif // OBJECT_H
