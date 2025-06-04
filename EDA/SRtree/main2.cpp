#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "MBB.h"
#include "Point.h"
#include "SRtree.h"
#include "Sphere.h"

int main() {
  Point p0({0.0f, 0.0f});
  Point p1({0.0f, 1.0f});
  Point p2({0.0f, 4.0f});
  Point p3({0.0f, 5.0f});

  SRTree tree(3);
  tree.insert(p0);
  tree.insert(p1);
  tree.insert(p2);
  tree.insert(p3);
  std::cout << "Root:\n"
            << *tree.getRoot() << std::endl;
}