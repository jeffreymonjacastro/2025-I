#include "is_convex.h"
#include <gtest/gtest.h>

TEST(Suite1, TestSquareTriangle) {
  vector<vector<int>> P = {{0, 0}, {1, 0}, {2, 0}, {1, 1}};
  EXPECT_TRUE(is_convex(P));
}

TEST(Suite2, DegenerateSquare) {
  vector<vector<int>> P = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
  EXPECT_FALSE(is_convex(P));
}
