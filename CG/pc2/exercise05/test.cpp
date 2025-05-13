#include "inside_polygon.h"
#include <gtest/gtest.h>

TEST(Suite1, TestTriangle) {
  EXPECT_TRUE(inside_polygon({{0, 0}, {4, 0}, {2, 4}}, 2, 2));
  EXPECT_FALSE(inside_polygon({{0, 0}, {4, 0}, {2, 4}}, 5, 5));
}

TEST(Suite1, TestSquare) {
  EXPECT_TRUE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, 2, 2));
  EXPECT_FALSE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, -1, -1));
}

TEST(Suite2, TestPointOnVertex) {
  EXPECT_TRUE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, 4, 4));
}

TEST(Suite2, TestRayIntersectsVertex) {
  EXPECT_FALSE(inside_polygon({{0, 0}, {4, 0}, {2, 4}}, 0, 4));
}

TEST(Suite3, TestPointOnEdge) {
  EXPECT_TRUE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, 2, 0));
  EXPECT_TRUE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, 2, 4));
}

TEST(Suite3, TestRayColinearToEdge) {
  EXPECT_FALSE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, -2, 0));
}

TEST(Suite4, TestPointOutside) {
  EXPECT_FALSE(inside_polygon({{0, 0}, {4, 0}, {4, 4}, {0, 4}}, 5, 5));
}

TEST(Suite5, TestArrowFigure) {
  vector<vector<int>> vertices(7, vector<int>(2));
  vertices[0] = {4, 2};
  vertices[1] = {8, 2};
  vertices[2] = {8, 4};
  vertices[3] = {10, 4};
  vertices[4] = {6, 8};
  vertices[5] = {2, 4};
  vertices[6] = {4, 4};
  EXPECT_TRUE(inside_polygon(vertices, 6, 4));
}
