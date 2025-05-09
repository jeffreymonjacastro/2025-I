#include "intersection.h"
#include <gtest/gtest.h>

TEST(Suite1, TestIntersectionIntegers) {
  EXPECT_TRUE(intersection(1, 1, 2, 2, 2, 2, 4, 4));
  EXPECT_TRUE(intersection(1, 1, 2, 2, 2, 2, 3, 3));
}

TEST(Suite2, TestIntersectionDoubles) {
  EXPECT_FALSE(intersection(-1.0, 0.0, 3.0, 3.0, 5.0, 4.5, 7.28, 6.21));
}

TEST(Suite3, TestBoundaryCases) {
  EXPECT_TRUE(intersection(INT_MIN, INT_MIN, INT_MAX, INT_MAX, INT_MIN,
                           INT_MIN + 1, INT_MAX, INT_MAX - 1));
}