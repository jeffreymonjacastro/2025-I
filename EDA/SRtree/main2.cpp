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
  Point p1({1.0f, 1.0f});
  Point p2({0.0f, 2.0f});

  SRTree tree(3);
  tree.insert(p0);
  std::cout << "Root: " << *tree.getRoot() << std::endl;
  tree.insert(p1);
  std::cout << "Root: " << *tree.getRoot() << std::endl;
  tree.insert(p2);
  std::cout << "Root: " << *tree.getRoot() << std::endl;
}