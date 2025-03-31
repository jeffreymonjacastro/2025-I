#include <iostream>
#include <vector>

using namespace std;

class Point {
private:
  int x;
  int y;

public:
  Point(int x, int y) : x(x), y(y) {}
  ~Point() {}
  int getX() const { return x; }
  int getY() const { return y; }
};

class Node {
private:
  Point *point;
  vector<Node *> children = vector<Node *>(4, nullptr);
  bool isRoot, isLeaf;

public:
  Node(Point *point) : point(point) { isLeaf = true; }
  ~Node() {
    delete point;
    for (Node *child : children) {
      delete child;
    }
  }

  void insert(Point *newPoint) {
    int cuadrant = this->getCuadrant(newPoint);

    if (this->children[cuadrant] == nullptr) {
      this->children[cuadrant] = new Node(newPoint);
      this->isLeaf = false;
    } else {
      this->children[cuadrant]->insert(newPoint);
    }
  }

  int getCuadrant(Point *newPoint) {
    if (newPoint->getX() < this->point->getX() &&
        newPoint->getY() >= this->point->getY()) {
      return 0; // NW
    } else if (newPoint->getX() >= this->point->getX() &&
               newPoint->getY() >= this->point->getY()) {
      return 1; // NE
    } else if (newPoint->getX() < this->point->getX() &&
               newPoint->getY() < this->point->getY()) {
      return 2; // SW
    } else {
      return 3; // SE
    }
  }

  void displayPretty(int level = 0) const {
    for (int i = 0; i < level; i++) {
      cout << "|  ";
    }
    cout << "+-- (" << point->getX() << ", " << point->getY() << ")" << endl;

    for (int i = 0; i < 4; i++) {
      if (children[i] != nullptr) {
        children[i]->displayPretty(level + 1);
      }
    }
  }
};

class QuadTree {
private:
  Node *root;
  int size;

public:
  QuadTree() { root = nullptr; }

  ~QuadTree() { delete root; }

  void insert(Point *point) {
    if (root == nullptr) {
      root = new Node(point);
      size = 1;
      return;
    }
    root->insert(point);
    size++;
  }

  int getSize() const { return size; }

  void displayPretty() const {
    if (root == nullptr) {
      cout << "QuadTree is empty." << endl;
    } else {
      cout << "QuadTree:" << endl;
      root->displayPretty();
    }
  }
};
