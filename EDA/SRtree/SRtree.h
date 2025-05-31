#ifndef SRTREE_H
#define SRTREE_H

#include "MBB.h"
#include "Point.h"
#include "Sphere.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class SRNode {
private:
  MBB _boundingBox;
  Sphere _boundingSphere;
  SRNode *_parent;
  vector<Point *> _points;
  vector<SRNode *> _children;
  bool _isLeaf;

public:
  bool getIsLeaf() const { return _isLeaf; }
  SRNode *getParent() const { return _parent; }

  const MBB &getBoundingBox() const { return _boundingBox; }
  const Sphere &getBoundingSphere() const { return _boundingSphere; }

  void setBoundingBox(const MBB &box) { _boundingBox = box; }
  void setBoundingSphere(const Sphere &sphere) { _boundingSphere = sphere; }
  void setParent(SRNode *parent) { _parent = parent; }
  void setIsLeaf(bool isLeaf) { _isLeaf = isLeaf; }

  // Insert algorithm
  SRNode *insert(Point &_data, std::size_t maxEntries);
};

class SRTree {
private:
  size_t _maxEntries;
  SRNode *_root;

public:
  void insert(const Point &point);
  bool search(const Point &point) const;
  vector<Point> rangeQuery(const MBB &box) const;
  vector<Point> rangeQuery(const Sphere &sphere) const;

  // k-nearest neighbors search
  vector<Point *> kNearestNeighbors(const Point &point, size_t k) const;
};

#endif // SRTREE_H