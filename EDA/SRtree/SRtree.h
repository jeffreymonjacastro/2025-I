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

#define MIN_PARTITION 0.3
#define MAX_PARTITION 0.7

class SRNode {
private:
  MBB _boundingBox;
  Sphere _boundingSphere;
  SRNode *_parent;
  std::vector<Point *> _points;
  std::vector<SRNode *> _children;
  bool _isLeaf;

public:
  bool getIsLeaf() const { return _isLeaf; }
  SRNode *getParent() const { return _parent; }

  const MBB &getBoundingBox() const { return _boundingBox; }
  const Sphere &getBoundingSphere() const { return _boundingSphere; }
  const std::vector<Point *> &getPoints() const { return _points; }
  const std::vector<SRNode *> &getChildren() const { return _children; }
  std::size_t getNumPoints() const { return _points.size(); }
  std::size_t getNumChildren() const { return _children.size(); }

  void setBoundingBox(const MBB &box) { _boundingBox = box; }
  void setBoundingSphere(const Sphere &sphere) { _boundingSphere = sphere; }
  void setParent(SRNode *parent) { _parent = parent; }
  void setIsLeaf(bool isLeaf) { _isLeaf = isLeaf; }

  void expandSphere(const Point &point);
  void expandSphere(const SRNode &child);

  void addPoint(Point *point);
  void addChild(SRNode *child);

  SRNode *splitLeaf(std::size_t maxEntries);
  SRNode *splitNode(std::size_t maxEntries);

  SRNode *chooseSubtree(const Point &point);

  SRNode &operator=(const SRNode &other);

  friend std::ostream &operator<<(std::ostream &os, const SRNode &node);

  // Insert algorithm
  SRNode *insert(const Point &_data, std::size_t maxEntries);
};

class SRTree {
private:
  SRNode *_root;
  std::size_t _maxEntries;

public:
  SRTree() : _maxEntries(15), _root(nullptr) {}
  explicit SRTree(std::size_t maxEntries) : _maxEntries(maxEntries), _root(nullptr) {}

  SRNode *getRoot() const { return _root; }

  void insert(const Point &point);
  bool search(const Point &point) const;
  std::vector<Point *> rangeQuery(const MBB &box) const;
  std::vector<Point *> rangeQuery(const Sphere &sphere) const;

  // k-nearest neighbors search
  std::vector<Point *> kNearestNeighbors(const Point &point, std::size_t k) const;
};

// ================== SRNode ========================

void SRNode::expandSphere(const Point &point) {
  // Hallar el centro como el promedio de los puntos
  Point center;
  for (auto &p : this->_points) {
    center += *p;
  }
  center /= this->getNumPoints();

  // Calcular el nuevo radio
  // r = min(ds, dr)
  // ds = max(|x - Ck|)
  // dr = MAXDIST(x, CR)

  float r = 0.0f;
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &p : this->_points) {
    float dist = Point::distance(center, *p);
    ds = std::max(ds, dist);
    dr = std::max(dr, MBB::maxDist(*p, this->_boundingBox));
  }

  r = std::min(ds, dr);

  this->_boundingSphere = Sphere(center, r);
}

void SRNode::expandSphere(const SRNode &child) {
  // Hallar el centro como el promedio ponderado de los centros de los hijos
  Point center;
  for (auto &c : this->_children) {
    center += c->getBoundingSphere().center * c->getNumPoints();
  }
  center /= this->getNumPoints();

  // Calcular el nuevo radio
  // r = min(ds, dr)
  // ds = max(|x - Ck| + Rk)
  // dr = MAXDIST(x, CR)

  float r = 0.0f;
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &c : this->_children) {
    Point childCenter = c->getBoundingSphere().center;
    float childRadius = c->getBoundingSphere().radius;
    float dist = Point::distance(center, childCenter);
    ds = std::max(ds, dist + childRadius);
    dr = std::max(dr, MBB::maxDist(childCenter, this->_boundingBox));
  }

  r = std::min(ds, dr);

  this->_boundingSphere = Sphere(center, r);
}

void SRNode::addPoint(Point *point) {
  this->_points.push_back(point);

  if (this->getNumPoints() == 1) {
    this->_boundingBox = MBB(*point);
    this->_boundingSphere = Sphere(*point, 0.0f);
  } else {
    this->_boundingBox.expandToInclude(*point);
    this->expandSphere(*point);
  }
}

void SRNode::addChild(SRNode *child) {
  // Añadir los puntos de este nodo a la esfera
  for (auto &p : child->getPoints()) {
    this->addPoint(p);
  }

  this->_children.push_back(child);
  child->setParent(this);

  if (this->_children.size() == 1) {
    this->_boundingBox = child->getBoundingBox();
    this->_boundingSphere = child->getBoundingSphere();
  } else {
    this->_boundingBox.expandToInclude(child->getBoundingBox());
    this->expandSphere(*child);
  }
}

SRNode *SRNode::splitLeaf(std::size_t maxEntries) {
  // Hallar la dimensión con mayor varianza
  float maxVariance = -std::numeric_limits<float>::max();
  std::size_t splitDim = 0;
  float n = this->getNumPoints();

  for (int i = 0; i < DIM; i++) {
    float mean = 0.0f;
    for (auto &point : _points) {
      mean += (*point)[i];
    }
    mean /= n;

    float variance = 0.0f;
    for (auto &point : _points) {
      float diff = (*point)[i] - mean;
      variance += diff * diff;
    }
    variance /= n;

    // std::cout << "Dimension " << i << ": media = " << mean << ", varianza = " << variance << "\n";

    if (variance > maxVariance) {
      maxVariance = variance;
      splitDim = i;
    }
  }

  // Ordenar los puntos por la dimensión de mayor varianza
  std::vector<Point *> sortedPoints = _points;

  std::sort(sortedPoints.begin(), sortedPoints.end(), [splitDim](const Point *a, const Point *b) {
    return (*a)[splitDim] < (*b)[splitDim];
  });

  // Evaluar divisiones
  float minCost = std::numeric_limits<float>::max();

  std::size_t minIdx = floor(MIN_PARTITION * n);
  std::size_t maxIdx = ceil(MAX_PARTITION * n);
  std::size_t idx = minIdx;

  for (std::size_t i = minIdx; i <= maxIdx; ++i) {
    SRNode left, right;
    float leftCost, rightCost;

    for (std::size_t j = 0; j < i; ++j) {
      left.addPoint(sortedPoints[j]);
    }
    leftCost = pow(left.getBoundingSphere().radius, DIM);

    for (std::size_t j = i; j < n; ++j) {
      right.addPoint(sortedPoints[j]);
    }
    rightCost = pow(right.getBoundingSphere().radius, DIM);

    if (leftCost + rightCost < minCost) {
      minCost = leftCost + rightCost;
      idx = i;
    }
  }

  // Crear el nuevo nodo
  SRNode *newNode = new SRNode();

  newNode->setParent(this->getParent());
  newNode->setIsLeaf(true);

  this->_points.clear();

  for (std::size_t i = 0; i < idx; ++i) {
    this->addPoint(sortedPoints[i]);
  }

  for (std::size_t i = idx; i < sortedPoints.size(); ++i) {
    newNode->addPoint(sortedPoints[i]);
  }

  return newNode;
}

SRNode *SRNode::chooseSubtree(const Point &point) {
  return nullptr;
}

SRNode *SRNode::insert(const Point &_data, std::size_t maxEntries) {
  if (this->getIsLeaf()) {
    if (this->getNumPoints() < maxEntries) {
      // Si el nodo hoja no está lleno, agregar el punto
      this->addPoint(new Point(_data));
      return nullptr;
    }

    // Si el nodo hoja está lleno, hacer un split
    this->addPoint(new Point(_data));
    SRNode *newNode = splitLeaf(maxEntries);

    return newNode;
  }

  // Si no es hoja, buscar el hijo adecuado
  // SRNode *bestChild = chooseSubtree(_data);
  return nullptr;
}

SRNode &SRNode::operator=(const SRNode &other) {
  if (this != &other) {
    _boundingBox = other._boundingBox;
    _boundingSphere = other._boundingSphere;
    _parent = other._parent;
    _points = other._points;
    _children = other._children;
    _isLeaf = other._isLeaf;
  }
  return *this;
}

std::ostream &operator<<(std::ostream &os, const SRNode &node) {
  static int indentLevel = 0;
  std::string indent(indentLevel * 2, ' ');

  os << indent << "- Bounding Box: " << node._boundingBox;
  os << indent << "- Bounding Sphere: " << node._boundingSphere;
  os << indent << "- Points: ";
  for (const auto &point : node._points) {
    os << *point << "; ";
  }
  os << "\n";

  os << indent << "- Children:\n";
  ++indentLevel;
  for (const auto &child : node._children) {
    os << *child;
  }
  --indentLevel;

  return os;
}

// ================== SRTree ========================

void SRTree::insert(const Point &point) {
  if (!_root) {
    _root = new SRNode();
    _root->setIsLeaf(true);
    _root->setParent(nullptr);
    _root->addPoint(new Point(point));
    return;
  }

  SRNode *child = _root->insert(point, _maxEntries);

  if (child) {
    // Si el insert devolvió un nuevo nodo, significa que hubo un split
    SRNode *newRoot = new SRNode();
    newRoot->setIsLeaf(false);
    newRoot->setParent(nullptr);
    newRoot->addChild(_root);
    newRoot->addChild(child);
    _root = newRoot;
  }
}

bool SRTree::search(const Point &point) const {
  return false;
}

std::vector<Point *> SRTree::rangeQuery(const MBB &box) const {
  return {};
}

std::vector<Point *> SRTree::rangeQuery(const Sphere &sphere) const {
  return {};
}

std::vector<Point *> SRTree::kNearestNeighbors(const Point &point, std::size_t k) const {
  return {};
}

#endif // SRTREE_H