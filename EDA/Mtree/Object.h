#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>

class Object {
public:
    explicit Object(std::string s)
        : value(std::move(s)) {}

    std::size_t distance(const Object& other) const {
        return levenshtein(value, other.value);
    }

    const std::string& str() const noexcept {
        return value;
    }

private:
    std::string value;

    // Distancia de Levenshtein
    static std::size_t levenshtein(const std::string& a,
                                   const std::string& b) {
        const std::size_t n = a.size();
        const std::size_t m = b.size();
        
        std::vector<std::vector<std::size_t>> dp(n + 1,
            std::vector<std::size_t>(m + 1));

        // Inicialización de bordes
        for (std::size_t i = 0; i <= n; ++i) dp[i][0] = i;
        for (std::size_t j = 0; j <= m; ++j) dp[0][j] = j;

        for (std::size_t i = 1; i <= n; ++i) {
            for (std::size_t j = 1; j <= m; ++j) {
                std::size_t cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
                dp[i][j] = std::min({
                    dp[i - 1][j] + 1,       // eliminación
                    dp[i][j - 1] + 1,       // inserción
                    dp[i - 1][j - 1] + cost // sustitución
                });
            }
        }
        return dp[n][m];
    }
};

#endif // OBJECT_H
