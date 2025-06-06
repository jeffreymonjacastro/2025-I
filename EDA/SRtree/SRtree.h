#ifndef SRTREE_H
#define SRTREE_H

#include "MBB.h"
#include "Point.h"
#include "Sphere.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#define MIN_PARTITION 0.4
#define MAX_PARTITION 0.6

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
  const vector<Point *> &getPoints() const { return _points; }
  const vector<SRNode *> &getChildren() const { return _children; }
  size_t getNumPoints() const;
  size_t getNumChildren() const { return _children.size(); }

  void setBoundingBox(const MBB &box) { _boundingBox = box; }
  void setBoundingSphere(const Sphere &sphere) { _boundingSphere = sphere; }
  void setParent(SRNode *parent) { _parent = parent; }
  void setIsLeaf(bool isLeaf) { _isLeaf = isLeaf; }

  void expandSphere(const Point &point);
  void expandSphere(const SRNode &child);

  void addPoint(Point *point);
  void addChild(SRNode *child);

  SRNode *splitLeaf(size_t maxEntries);
  SRNode *splitNode(size_t maxEntries);

  SRNode *chooseSubtree(const Point &point);

  void recalculateBoundingBox();

  friend ostream &operator<<(ostream &os, const SRNode &node);

  // Insert algorithm
  SRNode *insert(const Point &_data, size_t maxEntries);
};

class SRTree {
private:
  SRNode *_root;
  size_t _maxEntries;

public:
  SRTree() : _maxEntries(15), _root(nullptr) {}
  explicit SRTree(size_t maxEntries) : _maxEntries(maxEntries), _root(nullptr) {}

  SRNode *getRoot() const { return _root; }

  void recalculateTree(SRNode *node);
  void insert(const Point &point);
  bool search(const Point &point) const;
  vector<Point *> rangeQuery(const MBB &box) const;
  vector<Point *> rangeQuery(const Sphere &sphere) const;

  // k-nearest neighbors search
  vector<Point *> kNearestNeighbors(const Point &q, size_t k) const;
};

// ================== SRNode ========================

size_t SRNode::getNumPoints() const {
  if (this->_isLeaf) {
    return this->_points.size();
  } else {
    size_t total = 0;
    for (auto &child : this->_children) {
      total += child->getNumPoints();
    }
    return total;
  }
}

void SRNode::recalculateBoundingBox() {
  if (this->_isLeaf) {
    // Para nodos hoja, recalcular basándose en puntos
    if (this->_points.empty())
      return;

    this->_boundingBox = MBB(*this->_points[0]);

    for (size_t i = 1; i < this->_points.size(); ++i) {
      this->_boundingBox.expandToInclude(*this->_points[i]);
    }
  } else {
    // Para nodos internos, recalcular basándose en hijos
    if (this->_children.empty())
      return;

    this->_boundingBox = this->_children[0]->getBoundingBox();

    for (size_t i = 1; i < this->_children.size(); ++i) {
      this->_boundingBox.expandToInclude(this->_children[i]->getBoundingBox());
    }
  }
}

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
  float minr = numeric_limits<float>::max();
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &p : this->_points) {
    float dist = Point::distance(center, *p);
    ds = max(ds, dist);
    dr = max(dr, MBB::maxDist(*p, this->_boundingBox));
    minr = min(minr, dist);
  }

  r = min(ds, dr);

  this->_boundingSphere = Sphere(center, r, minr);
}

void SRNode::expandSphere(const SRNode &child) {
  // Hallar el centro como el promedio ponderado de los centros de los hijos
  Point center;
  size_t totalChildPoints = 0;
  for (auto &c : this->_children) {
    center += c->getBoundingSphere().center * c->getNumPoints();
    totalChildPoints += c->getNumPoints();
  }
  center /= totalChildPoints;

  // Calcular el nuevo radio
  // r = min(ds, dr)
  // ds = max(|x - Ck| + Rk)
  // dr = MAXDIST(x, CR)

  float r = 0.0f;
  float minr = numeric_limits<float>::max();
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &c : this->_children) {
    Point childCenter = c->getBoundingSphere().center;
    float childRadius = c->getBoundingSphere().radius;
    float dist = Point::distance(center, childCenter);
    ds = max(ds, dist + childRadius);
    dr = max(dr, MBB::maxDist(center, c->getBoundingBox()));
    minr = min(minr, max(dist - childRadius, 0.0f));
  }

  r = min(ds, dr);

  this->_boundingSphere = Sphere(center, r, minr);
}

void SRNode::addPoint(Point *point) {
  this->_points.push_back(point);

  // cout << "AddPoint: " << this->_points.size() << " puntos en el nodo.\n";

  if (this->getNumPoints() < 2) {
    this->_boundingBox = MBB(*point);
    this->_boundingSphere = Sphere(*point, 0.0f, 0.0f);
  } else {
    this->_boundingBox.expandToInclude(*point);
    this->expandSphere(*point);
  }
}

void SRNode::addChild(SRNode *child) {
  this->_children.push_back(child);
  child->setParent(this);

  if (this->_children.size() < 2) {
    this->_boundingBox = child->getBoundingBox();
    this->_boundingSphere = child->getBoundingSphere();
  } else {
    this->_boundingBox.expandToInclude(child->getBoundingBox());
    this->expandSphere(*child);
  }
}

SRNode *SRNode::splitLeaf(size_t maxEntries) {
  // Hallar la dimensión con mayor varianza
  float maxVariance = -numeric_limits<float>::max();
  size_t splitDim = 0;
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

    // cout << "Dimension " << i << ": media = " << mean << ", varianza = " << variance << "\n";

    if (variance > maxVariance) {
      maxVariance = variance;
      splitDim = i;
    }
  }

  // cout << "SplitLeaf - Dimension con mayor varianza: " << splitDim << " (varianza = " << maxVariance << ")\n";

  // Ordenar los puntos por la dimensión de mayor varianza
  vector<Point *> sortedPoints = _points;

  sort(sortedPoints.begin(), sortedPoints.end(), [splitDim](const Point *a, const Point *b) {
    return (*a)[splitDim] < (*b)[splitDim];
  });

  // Evaluar divisiones
  float minCost = numeric_limits<float>::max();

  size_t minIdx = floor(MIN_PARTITION * n);
  size_t maxIdx = ceil(MAX_PARTITION * n);
  size_t idx = minIdx;

  for (size_t i = minIdx; i <= maxIdx; ++i) {
    SRNode left, right;
    float leftCost, rightCost;

    for (size_t j = 0; j < i; ++j) {
      left.addPoint(sortedPoints[j]);
    }
    leftCost = left.getBoundingSphere().radius;

    for (size_t j = i; j < n; ++j) {
      right.addPoint(sortedPoints[j]);
    }
    rightCost = right.getBoundingSphere().radius;

    // cout << "Evaluando division en indice " << i << " minCost = " << minCost << "newCost = "
    // << leftCost + rightCost << "\n";

    if (leftCost + rightCost < minCost) {
      minCost = leftCost + rightCost;
      idx = i;
    }
  }

  // cout << "Mejor division en indice " << idx << " (costo = " << minCost << ")\n";

  // Crear el nuevo nodo
  SRNode *newNode = new SRNode();

  newNode->setParent(this->getParent());
  newNode->setIsLeaf(true);

  // Resetear el nodo actual
  this->_points.clear();

  bool first = true;
  for (size_t i = 0; i < idx; ++i) {
    if (first) {
      this->_boundingBox = MBB(*sortedPoints[i]);
      this->_boundingSphere = Sphere(*sortedPoints[i], 0.0f, 0.0f);
      this->_points.push_back(sortedPoints[i]);
      first = false;
    } else {
      this->addPoint(sortedPoints[i]);
    }
  }

  for (size_t i = idx; i < sortedPoints.size(); ++i) {
    newNode->addPoint(sortedPoints[i]);
  }

  // cout << "Split: " << this->getNumPoints() << " -> "
  // << newNode->getNumPoints() << " (idx=" << idx << ")" << endl;

  return newNode;
}

SRNode *SRNode::splitNode(size_t maxEntries) {
  // Hallar la dimensión con mayor varianza de los centros de los hijos
  float maxVariance = -numeric_limits<float>::max();
  size_t splitDim = 0;
  float n = this->getNumChildren();

  for (int i = 0; i < DIM; i++) {
    float mean = 0.0f;
    for (auto &child : _children) {
      mean += child->getBoundingSphere().center[i];
    }
    mean /= n;

    float variance = 0.0f;
    for (auto &child : _children) {
      float diff = child->getBoundingSphere().center[i] - mean;
      variance += diff * diff;
    }
    variance /= n;

    if (variance > maxVariance) {
      maxVariance = variance;
      splitDim = i;
    }
  }

  // cout << "SplitNode - Dimension con mayor varianza: " << splitDim << " (varianza = " << maxVariance << ")\n";

  // Ordenar los hijos por la dimensión de mayor varianza
  vector<SRNode *> sortedChildren = _children;

  sort(sortedChildren.begin(), sortedChildren.end(), [splitDim](const SRNode *a, const SRNode *b) {
    return a->getBoundingSphere().center[splitDim] < b->getBoundingSphere().center[splitDim];
  });

  // Evaluar divisiones
  float minCost = numeric_limits<float>::max();

  size_t minIdx = floor(MIN_PARTITION * n);
  size_t maxIdx = ceil(MAX_PARTITION * n);
  size_t idx = minIdx;

  for (size_t i = minIdx; i <= maxIdx; ++i) {
    SRNode left, right;
    float leftCost, rightCost;

    for (size_t j = 0; j < i; ++j) {
      left.addChild(sortedChildren[j]);
    }
    leftCost = left.getBoundingSphere().radius;

    for (size_t j = i; j < n; ++j) {
      right.addChild(sortedChildren[j]);
    }
    rightCost = right.getBoundingSphere().radius;

    if (leftCost + rightCost < minCost) {
      minCost = leftCost + rightCost;
      idx = i;
    }
  }

  // Crear el nuevo nodo
  SRNode *newNode = new SRNode();

  newNode->setParent(this->getParent());
  newNode->setIsLeaf(false);

  // Resetear el nodo actual
  this->_children.clear();
  this->_points.clear();

  bool first = true;
  for (size_t i = 0; i < idx; ++i) {
    if (first) {
      this->_boundingBox = sortedChildren[i]->getBoundingBox();
      this->_boundingSphere = sortedChildren[i]->getBoundingSphere();
      this->_children.push_back(sortedChildren[i]);
      sortedChildren[i]->setParent(this);
      first = false;
    } else {
      this->addChild(sortedChildren[i]);
    }
  }

  for (size_t i = idx; i < sortedChildren.size(); ++i) {
    newNode->addChild(sortedChildren[i]);
  }

  return newNode;
}

SRNode *SRNode::chooseSubtree(const Point &point) {
  SRNode *bestChild = nullptr;
  float minDist = numeric_limits<float>::max();

  for (auto &child : this->getChildren()) {
    // Distancia entre el punto y el centro del hijo
    Point childCenter = child->getBoundingSphere().center;
    float dist = Point::distance(point, childCenter);

    if (dist < minDist) {
      minDist = dist;
      bestChild = child;
    } else if (dist == minDist) {
      // Si hay empate, elegir el que tenga el radio mínimo
      if (child->getBoundingSphere().radius < bestChild->getBoundingSphere().radius) {
        bestChild = child;
      }
    }
  }

  if (bestChild->getIsLeaf()) {
    // Si el mejor hijo es una hoja, devolverlo
    return bestChild;
  } else {
    // Si no es una hoja, seguir buscando
    return bestChild->chooseSubtree(point);
  }
}

SRNode *SRNode::insert(const Point &_data, size_t maxEntries) {
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
  SRNode *bestChild = chooseSubtree(_data);

  // Insertar el punto en el hijo adecuado
  SRNode *newChild = bestChild->insert(_data, maxEntries);

  // Si el hijo devuelve un nuevo nodo, agregarlo al nodo actual
  if (newChild != nullptr) {
    this->addChild(newChild);

    // Recalculo Cardiaco
    this->recalculateBoundingBox();

    // Verificar si el nodo actual está lleno y realizar un split si es necesario
    if (this->getNumChildren() > maxEntries) {
      return splitNode(maxEntries);
    }

    return nullptr;
  }

  // Si no se devuelve un nuevo nodo, expandir el nodo actual
  this->_boundingBox.expandToInclude(_data);

  return nullptr;
}

ostream &operator<<(ostream &os, const SRNode &node) {
  static int indentLevel = 0;
  string indent(indentLevel * 2, ' ');

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

void SRTree::recalculateTree(SRNode *node) {
  if (node->getIsLeaf()) {
    node->recalculateBoundingBox();
  } else {
    // Recalcular hijos
    for (auto &child : node->getChildren()) {
      recalculateTree(child);
    }
    // Recalcular el nodo actual
    node->recalculateBoundingBox();
  }
}

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

  // Recalcular el árbol desde la raíz hacia abajo
  recalculateTree(_root);
}

bool SRTree::search(const Point &point) const {
  if (!_root) {
    return false;
  }

  priority_queue<pair<float, const SRNode *>, vector<pair<float, const SRNode *>>, greater<pair<float, const SRNode *>>> pq;

  // Agregar la raíz con distancia 0 (o distancia real al centro)
  float rootDist = Point::distance(point, _root->getBoundingSphere().center);
  pq.push({rootDist, _root});

  while (!pq.empty()) {
    auto [dist, node] = pq.top();
    pq.pop();

    if (node->getIsLeaf()) {
      for (const Point *p : node->getPoints()) {
        bool match = true;
        for (size_t i = 0; i < DIM; ++i) {
          if (abs((*p)[i] - point[i]) > EPSILON) {
            match = false;
            break;
          }
        }
        if (match) {
          return true;
        }
      }
    } else {
      for (const SRNode *child : node->getChildren()) {
        float childDist = Point::distance(point, child->getBoundingSphere().center);
        pq.push({childDist, child});
      }
    }
  }

  return false;
}

vector<Point *> SRTree::rangeQuery(const MBB &box) const {
  if (!_root) {
    return {};
  }

  function<bool(const MBB &)> intersects = [&box](const MBB &other) {
    for (size_t i = 0; i < DIM; ++i) {
      if (box.maxCorner[i] < other.minCorner[i] || box.minCorner[i] > other.maxCorner[i]) {
        return false;
      }
    }
    return true;
  };

  function<bool(const Point &)> pointInBox = [&box](const Point &p) {
    for (size_t i = 0; i < DIM; ++i) {
      if (p[i] < box.minCorner[i] - EPSILON || p[i] > box.maxCorner[i] + EPSILON) {
        return false;
      }
    }
    return true;
  };

  function<float(const Point &)> distanceToBoxCenter = [&box](const Point &p) {
    Point boxCenter;
    for (size_t i = 0; i < DIM; ++i) {
      boxCenter[i] = (box.minCorner[i] + box.maxCorner[i]) / 2.0f;
    }
    return Point::distance(p, boxCenter);
  };

  vector<Point *> res;
  priority_queue<pair<float, const SRNode *>, vector<pair<float, const SRNode *>>, greater<pair<float, const SRNode *>>> pq;

  if (intersects(_root->getBoundingBox())) {
    pq.push({0.0f, _root});
  }

  while (!pq.empty()) {
    auto [dist, node] = pq.top();
    pq.pop();

    if (node->getIsLeaf()) {
      for (Point *p : node->getPoints()) {
        if (pointInBox(*p)) {
          res.push_back(p);
        }
      }
    } else {
      for (const SRNode *child : node->getChildren()) {
        if (intersects(child->getBoundingBox())) {
          float childDist = distanceToBoxCenter(child->getBoundingSphere().center);
          pq.push({childDist, child});
        }
      }
    }
  }

  return res;
}

vector<Point *> SRTree::rangeQuery(const Sphere &sphere) const {
  if (!_root) {
    return {};
  }

  function<bool(const Sphere &)> intersects = [&sphere](const Sphere &other) {
    float dist = Point::distance(sphere.center, other.center);
    return dist <= (sphere.radius + other.radius);
  };

  function<bool(const Point &)> pointInSphere = [&sphere](const Point &p) {
    float dist = Point::distance(p, sphere.center);
    return dist <= sphere.radius + EPSILON;
  };

  vector<Point *> res;
  priority_queue<pair<float, const SRNode *>, vector<pair<float, const SRNode *>>, greater<pair<float, const SRNode *>>> pq;

  if (intersects(_root->getBoundingSphere())) {
    pq.push({0.0f, _root});
  }

  while (!pq.empty()) {
    auto [dist, node] = pq.top();
    pq.pop();

    if (node->getIsLeaf()) {
      for (Point *p : node->getPoints()) {
        if (pointInSphere(*p)) {
          res.push_back(p);
        }
      }
    } else {
      for (const SRNode *child : node->getChildren()) {
        if (intersects(child->getBoundingSphere())) {
          pq.push({Point::distance(sphere.center, child->getBoundingSphere().center), child});
        }
      }
    }
  }

  return res;
}

vector<Point *> SRTree::kNearestNeighbors(const Point &q, size_t k) const {
  struct KNN {
    Point *p;
    float dist;

    KNN(Point *p, float d) : p(p), dist(d) {}

    bool operator<(const KNN &other) const {
      return dist < other.dist;
    }
  };

  if (!_root || k == 0) {
    return {};
  }

  priority_queue<KNN> L;
  float Dk = numeric_limits<float>::max();

  function<void(Point *, float, size_t, float &)> insertKNN;
  insertKNN = [&L](Point *point, float distance, size_t k, float &Dk) {
    if (L.size() < k) {
      L.push(KNN(point, distance));
      if (L.size() == k) {
        Dk = L.top().dist;
      }
    } else if (distance < Dk) {
      L.pop();
      L.push(KNN(point, distance));
      Dk = L.top().dist;
    }
  };

  function<void(const Point &, size_t, const SRNode *, float &)> fndftrav;
  fndftrav = [&](const Point &q, size_t k, const SRNode *e, float &Dk) {
    if (e->getIsLeaf()) {
      Point Mp = e->getBoundingSphere().center;

      for (Point *o : e->getPoints()) {
        float distQM = Point::distance(q, Mp);
        float distOM = Point::distance(*o, Mp);

        // Pruning Rule 2: d(q,M) - d(o,M) > Dk
        if (distQM - distOM > Dk) {
          continue;
        }

        // Pruning Rule 4: d(o,M) - d(q,M) > Dk
        if (distOM - distQM > Dk) {
          continue;
        }

        float distQO = Point::distance(q, *o);
        if (L.size() < k || distQO < Dk) {
          insertKNN(o, distQO, k, Dk);
        }
      }
    } else {
      // MEANORPIVOTDIST
      vector<pair<float, const SRNode *>> activeList;

      for (const SRNode *child : e->getChildren()) {
        Point Mp = child->getBoundingSphere().center;
        float distQMp = Point::distance(q, Mp);
        activeList.push_back({distQMp, child});
      }

      // Ordenar
      sort(activeList.begin(), activeList.end());

      // Pruning Rule 5: Solo para k=1 y si mejora la estimación
      if (k == 1 && Dk == numeric_limits<float>::max()) {
        for (const auto &[distQMp, child] : activeList) {
          float rp_min = child->getBoundingSphere().minradius;
          float possibleBest = distQMp + rp_min;

          if (possibleBest < Dk) {
            Dk = possibleBest;
            break;
          }
        }
      }

      for (const auto &[distQMp, child] : activeList) {
        float rp_max = child->getBoundingSphere().radius;
        float rp_min = child->getBoundingSphere().minradius;

        // Pruning Rule 1: d(q,Mp) - rp,max > Dk
        if (distQMp - rp_max > Dk) {
          continue;
        }

        // Pruning Rule 3: rp,min - d(q,Mp) > Dk
        if (rp_min - distQMp > Dk) {
          continue;
        }

        fndftrav(q, k, child, Dk);
      }
    }
  };

  // FNDFTRAV
  fndftrav(q, k, _root, Dk);

  vector<Point *> res;

  res.reserve(L.size());
  while (!L.empty()) {
    res.push_back(L.top().p);
    L.pop();
  }

  reverse(res.begin(), res.end());

  return res;
}

#endif // SRTREE_H