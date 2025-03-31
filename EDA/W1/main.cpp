#include "PointQuadTree.h"

int main() {
  Point *p1 = new Point(2, 2);
  Point *p2 = new Point(4, 4);
  Point *p3 = new Point(3, 3);

  QuadTree *QT = new QuadTree();
  QT->insert(p1);
  QT->insert(p2);
  QT->insert(p3);

  QT->displayPretty();

  return 0;
}