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
  std::size_t getNumPoints() const;
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

  void recalculateBoundingBox();

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

  void recalculateTree(SRNode *node);
  void insert(const Point &point);
  bool search(const Point &point) const;
  std::vector<Point *> rangeQuery(const MBB &box) const;
  std::vector<Point *> rangeQuery(const Sphere &sphere) const;

  // k-nearest neighbors search
  std::vector<Point *> kNearestNeighbors(const Point &q, std::size_t k) const;
};

// ================== SRNode ========================

std::size_t SRNode::getNumPoints() const {
  if (this->_isLeaf) {
    return this->_points.size();
  } else {
    std::size_t total = 0;
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

    for (std::size_t i = 1; i < this->_points.size(); ++i) {
      this->_boundingBox.expandToInclude(*this->_points[i]);
    }
  } else {
    // Para nodos internos, recalcular basándose en hijos
    if (this->_children.empty())
      return;

    this->_boundingBox = this->_children[0]->getBoundingBox();

    for (std::size_t i = 1; i < this->_children.size(); ++i) {
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
  float minr = std::numeric_limits<float>::max();
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &p : this->_points) {
    float dist = Point::distance(center, *p);
    ds = std::max(ds, dist);
    dr = std::max(dr, MBB::maxDist(*p, this->_boundingBox));
    minr = std::min(minr, dist);
  }

  r = std::min(ds, dr);

  this->_boundingSphere = Sphere(center, r, minr);
}

void SRNode::expandSphere(const SRNode &child) {
  // Hallar el centro como el promedio ponderado de los centros de los hijos
  Point center;
  std::size_t totalChildPoints = 0;
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
  float minr = std::numeric_limits<float>::max();
  float ds = 0.0f;
  float dr = 0.0f;

  for (auto &c : this->_children) {
    Point childCenter = c->getBoundingSphere().center;
    float childRadius = c->getBoundingSphere().radius;
    float dist = Point::distance(center, childCenter);
    ds = std::max(ds, dist + childRadius);
    dr = std::max(dr, MBB::maxDist(center, c->getBoundingBox()));
    minr = std::min(minr, std::max(dist - childRadius, 0.0f));
  }

  r = std::min(ds, dr);

  this->_boundingSphere = Sphere(center, r, minr);
}

void SRNode::addPoint(Point *point) {
  this->_points.push_back(point);

  // std::cout << "AddPoint: " << this->_points.size() << " puntos en el nodo.\n";

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

  // std::cout << "SplitLeaf - Dimension con mayor varianza: " << splitDim << " (varianza = " << maxVariance << ")\n";

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
    leftCost = left.getBoundingSphere().radius;

    for (std::size_t j = i; j < n; ++j) {
      right.addPoint(sortedPoints[j]);
    }
    rightCost = right.getBoundingSphere().radius;

    // std::cout << "Evaluando division en indice " << i << " minCost = " << minCost << "newCost = "
    // << leftCost + rightCost << "\n";

    if (leftCost + rightCost < minCost) {
      minCost = leftCost + rightCost;
      idx = i;
    }
  }

  // std::cout << "Mejor division en indice " << idx << " (costo = " << minCost << ")\n";

  // Crear el nuevo nodo
  SRNode *newNode = new SRNode();

  newNode->setParent(this->getParent());
  newNode->setIsLeaf(true);

  // Resetear el nodo actual
  this->_points.clear();
  // this->_boundingBox = MBB();
  // this->_boundingSphere = Sphere();

  // for (std::size_t i = 0; i < idx; ++i) {
  //   this->addPoint(sortedPoints[i]);
  // }

  bool first = true;
  for (std::size_t i = 0; i < idx; ++i) {
    if (first) {
      this->_boundingBox = MBB(*sortedPoints[i]);
      this->_boundingSphere = Sphere(*sortedPoints[i], 0.0f, 0.0f);
      this->_points.push_back(sortedPoints[i]);
      first = false;
    } else {
      this->addPoint(sortedPoints[i]);
    }
  }

  for (std::size_t i = idx; i < sortedPoints.size(); ++i) {
    newNode->addPoint(sortedPoints[i]);
  }

  // std::cout << "Split: " << this->getNumPoints() << " -> "
  // << newNode->getNumPoints() << " (idx=" << idx << ")" << std::endl;

  return newNode;
}

SRNode *SRNode::splitNode(std::size_t maxEntries) {
  // Hallar la dimensión con mayor varianza de los centros de los hijos
  float maxVariance = -std::numeric_limits<float>::max();
  std::size_t splitDim = 0;
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

  // std::cout << "SplitNode - Dimension con mayor varianza: " << splitDim << " (varianza = " << maxVariance << ")\n";

  // Ordenar los hijos por la dimensión de mayor varianza
  std::vector<SRNode *> sortedChildren = _children;

  std::sort(sortedChildren.begin(), sortedChildren.end(), [splitDim](const SRNode *a, const SRNode *b) {
    return a->getBoundingSphere().center[splitDim] < b->getBoundingSphere().center[splitDim];
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
      left.addChild(sortedChildren[j]);
    }
    leftCost = left.getBoundingSphere().radius;

    for (std::size_t j = i; j < n; ++j) {
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
  // this->_boundingBox = MBB();
  // this->_boundingSphere = Sphere();

  // for (std::size_t i = 0; i < idx; ++i) {
  //   this->addChild(sortedChildren[i]);
  // }

  bool first = true;
  for (std::size_t i = 0; i < idx; ++i) {
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

  for (std::size_t i = idx; i < sortedChildren.size(); ++i) {
    newNode->addChild(sortedChildren[i]);
  }

  return newNode;
}

SRNode *SRNode::chooseSubtree(const Point &point) {
  SRNode *bestChild = nullptr;
  float minDist = std::numeric_limits<float>::max();

  for (auto &child : this->getChildren()) {
    // Distancia entre el punto y el centro del hijo
    Point childCenter = child->getBoundingSphere().center;
    float dist = Point::distance(point, childCenter);

    if (dist < minDist) {
      minDist = dist;
      bestChild = child;
    } else if (dist == minDist) {
      // Si hay empate, elegir el que tenga el área mínima
      if (child->getBoundingSphere().radius < bestChild->getBoundingSphere().radius) {
        bestChild = child;
      }
    }
  }

  if (bestChild->getIsLeaf()) {
    // Si el mejor hijo es una hoja, devolverlo
    return bestChild;
  } else {
    // Si no es una hoja, seguir buscando en sus hijos
    return bestChild->chooseSubtree(point);
  }
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

  std::priority_queue<std::pair<float, const SRNode *>, std::vector<std::pair<float, const SRNode *>>, std::greater<std::pair<float, const SRNode *>>> pq;

  // Agregar la raíz con distancia 0 (o distancia real al centro)
  float rootDist = Point::distance(point, _root->getBoundingSphere().center);
  pq.push({rootDist, _root});

  while (!pq.empty()) {
    auto [dist, node] = pq.top();
    pq.pop();

    bool inBoundingBox = true;
    for (std::size_t i = 0; i < DIM; ++i) {
      if (point[i] < node->getBoundingBox().minCorner[i] - EPSILON ||
          point[i] > node->getBoundingBox().maxCorner[i] + EPSILON) {
        inBoundingBox = false;
        break;
      }
    }

    if (!inBoundingBox) {
      continue;
    }

    if (node->getIsLeaf()) {
      for (const Point *p : node->getPoints()) {
        bool match = true;
        for (std::size_t i = 0; i < DIM; ++i) {
          if (std::abs((*p)[i] - point[i]) > EPSILON) {
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

std::vector<Point *> SRTree::rangeQuery(const MBB &box) const {
  if (!_root) {
    return {};
  }

  std::function<bool(const MBB &)> intersects = [&box](const MBB &other) {
    for (std::size_t i = 0; i < DIM; ++i) {
      if (box.maxCorner[i] < other.minCorner[i] || box.minCorner[i] > other.maxCorner[i]) {
        return false;
      }
    }
    return true;
  };

  std::function<bool(const Point &)> pointInBox = [&box](const Point &p) {
    for (std::size_t i = 0; i < DIM; ++i) {
      if (p[i] < box.minCorner[i] - EPSILON || p[i] > box.maxCorner[i] + EPSILON) {
        return false;
      }
    }
    return true;
  };

  std::function<float(const Point &)> distanceToBoxCenter = [&box](const Point &p) {
    Point boxCenter;
    for (std::size_t i = 0; i < DIM; ++i) {
      boxCenter[i] = (box.minCorner[i] + box.maxCorner[i]) / 2.0f;
    }
    return Point::distance(p, boxCenter);
  };

  std::vector<Point *> res;
  std::priority_queue<std::pair<float, const SRNode *>, std::vector<std::pair<float, const SRNode *>>, std::greater<std::pair<float, const SRNode *>>> pq;

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

std::vector<Point *> SRTree::rangeQuery(const Sphere &sphere) const {
  if (!_root) {
    return {};
  }

  std::function<bool(const Sphere &)> intersects = [&sphere](const Sphere &other) {
    float dist = Point::distance(sphere.center, other.center);
    return dist <= (sphere.radius + other.radius);
  };

  std::function<bool(const Point &)> pointInSphere = [&sphere](const Point &p) {
    float dist = Point::distance(p, sphere.center);
    return dist <= sphere.radius + EPSILON;
  };

  std::vector<Point *> res;
  std::priority_queue<std::pair<float, const SRNode *>, std::vector<std::pair<float, const SRNode *>>, std::greater<std::pair<float, const SRNode *>>> pq;

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

std::vector<Point *> SRTree::kNearestNeighbors(const Point &q, std::size_t k) const {
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

  std::priority_queue<KNN> L;
  float Dk = std::numeric_limits<float>::max();

  std::function<void(Point *, float, std::size_t, float &)> insertKNN;
  insertKNN = [&L](Point *point, float distance, std::size_t k, float &Dk) {
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

  std::function<void(const Point &, std::size_t, const SRNode *, float &)> fndftrav;
  fndftrav = [&](const Point &q, std::size_t k, const SRNode *e, float &Dk) {
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
      std::vector<std::pair<float, const SRNode *>> activeList;

      for (const SRNode *child : e->getChildren()) {
        Point Mp = child->getBoundingSphere().center;
        float distQMp = Point::distance(q, Mp);
        activeList.push_back({distQMp, child});
      }

      // Ordenar
      std::sort(activeList.begin(), activeList.end());

      // Pruning Rule 5: Solo para k=1 y si mejora la estimación
      if (k == 1 && Dk == std::numeric_limits<float>::max()) {
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

  std::vector<Point *> res;

  res.reserve(L.size());
  while (!L.empty()) {
    res.push_back(L.top().p);
    L.pop();
  }

  std::reverse(res.begin(), res.end());

  return res;
}

#endif // SRTREE_H