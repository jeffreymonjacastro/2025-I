#include "jarvis_graham.h"
#include <gtest/gtest.h>

TEST(Suite1, testJarvis) {
  int n = 6;
  vector<vector<int>> vertices(n, vector<int>(2));

  vertices[0] = {0, 1};
  vertices[1] = {1, 2};
  vertices[2] = {2, 1};
  vertices[3] = {1, 0};
  vertices[4] = {3, 1};
  vertices[5] = {2, 0};

  vector<vector<int>> expected_hull = {{1, 0}, {2, 0}, {3, 1}, {1, 2}, {0, 1}};

  vector<vector<int>> result_hull = jarvis(vertices, false);

  EXPECT_EQ(result_hull.size(), expected_hull.size());
  for (size_t i = 0; i < result_hull.size(); ++i) {
    EXPECT_EQ(result_hull[i][0], expected_hull[i][0]);
    EXPECT_EQ(result_hull[i][1], expected_hull[i][1]);
  }
}
