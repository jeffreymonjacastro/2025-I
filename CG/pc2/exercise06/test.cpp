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
  vector<vector<int>> jarvis_hull = jarvis(vertices, false);

  EXPECT_EQ(jarvis_hull.size(), expected_hull.size());
  for (size_t i = 0; i < jarvis_hull.size(); ++i) {
    EXPECT_EQ(jarvis_hull[i][0], expected_hull[i][0]);
    EXPECT_EQ(jarvis_hull[i][1], expected_hull[i][1]);
  }
}

TEST(Suite1, testGraham) {
  int n = 6;
  vector<vector<int>> vertices(n, vector<int>(2));

  vertices[0] = {0, 1};
  vertices[1] = {1, 2};
  vertices[2] = {2, 1};
  vertices[3] = {1, 0};
  vertices[4] = {3, 1};
  vertices[5] = {2, 0};

  vector<vector<int>> expected_hull = {{1, 0}, {2, 0}, {3, 1}, {1, 2}, {0, 1}};
  vector<vector<int>> graham_hull = graham(vertices, false);

  EXPECT_EQ(graham_hull.size(), expected_hull.size());
  for (size_t i = 0; i < graham_hull.size(); ++i) {
    EXPECT_EQ(graham_hull[i][0], expected_hull[i][0]);
    EXPECT_EQ(graham_hull[i][1], expected_hull[i][1]);
  }
}
