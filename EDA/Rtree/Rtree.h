#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

typedef unsigned int uint;
typedef unsigned char uchar;

class Point {
public:
  float x, y;
  Point() : x(0.0f), y(0.0f) {}
  Point(float x, float y) : x(x), y(y) {}

  // Distancia euclidiana
  float distanceTo(const Point &other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
  }
};

class RNode;

class MBB {
public:
  Point lower;  // Esquina inferior izquierda
  Point upper;  // Esquina superior derecha
  bool isEmpty; // Si es hoja o no

  MBB() : lower(Point()), upper(Point()), isEmpty(true) {}
  MBB(const Point &p1, const Point &p2)
      : lower(Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y))),
        upper(Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y))),
        isEmpty(false) {}

  float area() const {
    float dx = upper.x - lower.x;
    float dy = upper.y - lower.y;
    return dx * dy;
  };

  float semiPerimeter() const {
    float dx = upper.x - lower.x;
    float dy = upper.y - lower.y;
    return dx + dy;
  };

  float distanceTo(const Point &p) const {
    float closestX = std::max(lower.x, std::min(p.x, upper.x));
    float closestY = std::max(lower.y, std::min(p.y, upper.y));
    float dx = p.x - closestX;
    float dy = p.y - closestY;
    return std::sqrt(dx * dx + dy * dy);
  };

  // Incremento del semiperimetro si agrega p
  float deltaSemiPerimeter(const Point &p) const {
    if (this->distanceTo(p) == 0)
      return 0;
    float upperx = std::max(p.x, upper.x);
    float lowerx = std::min(p.x, lower.x);
    float uppery = std::max(p.y, upper.y);
    float lowery = std::min(p.y, lower.y);

    float semiPerimeter = (upperx - lowerx) + (uppery - lowery);

    return semiPerimeter - this->semiPerimeter();
  };

  // Incremento del area si agrega MBB
  float deltaArea(const MBB &other) const {
    MBB newMBB = this->unionOf(*this, other);
    float currentArea = this->area() + other.area() - this->intersects(other);
    return newMBB.area() - currentArea;
  };

  // Expande para incluir point o MBB
  void expandToInclude(const Point &p) {
    if (this->distanceTo(p) == 0)
      return;
    lower.x = std::min(lower.x, p.x);
    lower.y = std::min(lower.y, p.y);
    upper.x = std::max(upper.x, p.x);
    upper.y = std::max(upper.y, p.y);
  };

  void expandToInclude(const MBB &other) {
    lower.x = std::min(lower.x, other.lower.x);
    lower.y = std::min(lower.y, other.lower.y);
    upper.x = std::max(upper.x, other.upper.x);
    upper.y = std::max(upper.y, other.upper.y);
  };

  // Retorna el area de interseccion
  float intersects(const MBB &other) const {
    float dx = std::max(0.0f, std::min(upper.x, other.upper.x) -
                                  std::max(lower.x, other.lower.x));
    float dy = std::max(0.0f, std::min(upper.y, other.upper.y) -
                                  std::max(lower.y, other.lower.y));
    return dx * dy;
  };

  // Crear MBB a partir de un vector de puntos
  static MBB computeFromPoints(const std::vector<Point> &pts) {
    if (pts.empty()) {
      return MBB();
    }

    if (pts.size() == 1) {
      return MBB(pts[0], pts[0]);
    }

    MBB res(pts[0], pts[1]);
    float minX = res.lower.x;
    float minY = res.lower.y;
    float maxX = res.upper.x;
    float maxY = res.upper.y;

    for (int i = 2; i < pts.size(); i++) {
      minX = std::min(minX, pts[i].x);
      minY = std::min(minY, pts[i].y);
      maxX = std::max(maxX, pts[i].x);
      maxY = std::max(maxY, pts[i].y);
    }

    return MBB(Point(minX, minY), Point(maxX, maxY));
  };

  // Crear MBB a partir de un vector de nodos
  static MBB computeFromNodes(const std::vector<RNode *> &nodes);

  // Union MBBs
  static MBB unionOf(const MBB &a, const MBB &b) {
    if (a.isEmpty && b.isEmpty)
      return MBB();

    if (a.isEmpty)
      return b;

    if (b.isEmpty)
      return a;

    Point plower(std::min(a.lower.x, b.lower.x),
                 std::min(a.lower.y, b.lower.y));
    Point pupper(std::max(a.upper.x, b.upper.x),
                 std::max(a.upper.y, b.upper.y));

    return MBB(plower, pupper);
  };
};

// -------------------------------
// Clase RNode
// -------------------------------
class RNode {
private:
  // Linear Split para nodos hoja
  RNode *linearSplitLeaf(uint maxEntries) {
    RNode *newNode = new RNode(true);
    uint minEntries = (maxEntries + 1) / 2;

    // Encontrar los dos puntos más lejanos
    int firstIndex = 0, secondIndex = 0;
    float maxDistance = -1;

    // Random
    int randomIndex = std::rand() % this->points.size();
    Point randomPoint = this->points[randomIndex];

    // Encontrar el punto más lejano al punto aleatorio
    for (int i = 0; i < this->points.size(); i++) {
      if (i == randomIndex)
        continue;
      float distance = randomPoint.distanceTo(this->points[i]);
      if (distance > maxDistance) {
        maxDistance = distance;
        firstIndex = i;
      }
    }

    Point firstPoint = this->points[firstIndex];
    this->points.erase(this->points.begin() + firstIndex);
    maxDistance = -1;

    // Encontrar el segundo punto más lejano al primero
    for (int i = 0; i < this->points.size(); ++i) {
      float distance = firstPoint.distanceTo(this->points[i]);
      if (distance > maxDistance) {
        maxDistance = distance;
        secondIndex = i;
      }
    }

    Point secondPoint = this->points[secondIndex];
    this->points.erase(this->points.begin() + secondIndex);

    std::vector<Point> remainingPoints = this->points;
    this->points.clear();

    newNode->points.push_back(secondPoint);
    newNode->mbr = MBB(secondPoint, secondPoint);
    this->points.push_back(firstPoint);
    this->mbr = MBB(firstPoint, firstPoint);

    // Asignar los puntos restantes a los grupos
    while (!remainingPoints.empty()) {
      Point currentPoint = remainingPoints.back();
      remainingPoints.pop_back();

      MBB firstMBB = this->mbr;
      MBB secondMBB = newNode->mbr;
      firstMBB.expandToInclude(currentPoint);
      secondMBB.expandToInclude(currentPoint);
      float increaseInOriginal = firstMBB.area() - this->mbr.area();
      float increaseInNew = secondMBB.area() - newNode->mbr.area();

      // Decidir a qué grupo asignar el punto
      if (increaseInOriginal < increaseInNew) {
        this->mbr = firstMBB;
        this->points.push_back(currentPoint);
      } else if (increaseInOriginal > increaseInNew) {
        newNode->mbr = secondMBB;
        newNode->points.push_back(currentPoint);
      } else {
        if (firstMBB.area() < secondMBB.area()) {
          this->mbr = firstMBB;
          this->points.push_back(currentPoint);
        } else if (secondMBB.area() > firstMBB.area()) {
          newNode->mbr = secondMBB;
          newNode->points.push_back(currentPoint);
        } else {
          if (this->points.size() < newNode->points.size()) {
            this->mbr = firstMBB;
            this->points.push_back(currentPoint);
          } else {
            newNode->mbr = secondMBB;
            newNode->points.push_back(currentPoint);
          }
        }
      }
    }

    this->balancePoints(newNode, minEntries);
    newNode->balancePoints(this, minEntries);

    this->mbr = MBB::computeFromPoints(this->points);
    newNode->mbr = MBB::computeFromPoints(newNode->points);

    return newNode;
  };

  // Quadratic Split para nodos internos
  RNode *quadraticSplitInternal(uint maxEntries) {
    RNode *newNode = new RNode(false);
    uint minEntries = (maxEntries + 1) / 2;

    // Encontrar los dos MBB que tengan la mayor area muerta
    int firstIndex = 0, secondIndex = 0;
    float maxDeadSpace = -1;
    for (int i = 0; i < this->children.size(); ++i) {
      for (int j = i + 1; j < this->children.size(); ++j) {
        MBB fusion =
            MBB::unionOf(this->children[i]->mbr, this->children[j]->mbr);
        float ocupedSpace =
            this->children[i]->mbr.area() + this->children[j]->mbr.area() - this->children[i]->mbr.intersects(children[j]->mbr);
        float deadSpace = fusion.area() - ocupedSpace;
        if (deadSpace > maxDeadSpace) {
          maxDeadSpace = deadSpace;
          firstIndex = i;
          secondIndex = j;
        }
      }
    }

    // Inicializar los dos grupos con las semillas
    RNode *firstChild = this->children[firstIndex];
    RNode *secondChild = this->children[secondIndex];

    // Remover las semillas de la lista original
    this->children.erase(this->children.begin() + secondIndex);
    this->children.erase(this->children.begin() + firstIndex);

    std::vector<RNode *> remainingChildren = this->children;
    this->children.clear();

    this->children.push_back(firstChild);
    this->mbr = firstChild->mbr;
    newNode->children.push_back(secondChild);
    newNode->mbr = secondChild->mbr;

    // Asignar los hijos restantes a los grupos
    while (!remainingChildren.empty()) {
      RNode *currentChild = remainingChildren.back();
      remainingChildren.pop_back();

      MBB firstMBB = this->mbr;
      MBB secondMBB = newNode->mbr;
      firstMBB.expandToInclude(currentChild->mbr);
      secondMBB.expandToInclude(currentChild->mbr);

      float increaseInOriginal = firstMBB.area() - this->mbr.area();
      float increaseInNew = secondMBB.area() - newNode->mbr.area();

      if (increaseInOriginal < increaseInNew) {
        this->children.push_back(currentChild);
        this->mbr = firstMBB;
      } else if (increaseInOriginal > increaseInNew) {
        newNode->children.push_back(currentChild);
        newNode->mbr = secondMBB;
      } else {
        if (this->mbr.area() < newNode->mbr.area()) {
          this->children.push_back(currentChild);
          this->mbr = firstMBB;
        } else if (this->mbr.area() > newNode->mbr.area()) {
          newNode->children.push_back(currentChild);
          newNode->mbr = secondMBB;
        } else {
          if (this->children.size() < newNode->points.size()) {
            this->children.push_back(currentChild);
            this->mbr = firstMBB;
          } else {
            newNode->children.push_back(currentChild);
            newNode->mbr = secondMBB;
          }
        }
      }
    }

    this->balanceNodes(newNode, minEntries);
    newNode->balanceNodes(this, minEntries);

    this->mbr = MBB::computeFromNodes(this->children);
    newNode->mbr = MBB::computeFromNodes(newNode->children);

    return newNode;
  };

  // Escoger el hijo adecuado para insertar el punto
  RNode *chooseSubtree(Point p) {
    RNode *bestChild = nullptr;
    float minIncrease = std::numeric_limits<float>::max();

    uint idx = -1;
    std::vector<uint> empates;

    for (int i = 0; i < this->children.size(); ++i) {
      float increase = this->children[i]->mbr.deltaSemiPerimeter(p);
      if (increase < minIncrease) {
        minIncrease = increase;
        idx = i;
      }
    }

    for (int i = 0; i < this->children.size(); ++i) {
      if (this->children[i]->mbr.deltaSemiPerimeter(p) == minIncrease) {
        empates.push_back(i);
      }
    }

    if (empates.empty()) {
      return this->children[idx];
    }

    float minArea = this->children[empates[0]]->mbr.area();
    for (int i = 1; i < empates.size(); ++i) {
      if (this->children[empates[i]]->mbr.area() < minArea) {
        idx = empates[i];
        minArea = this->children[empates[i]]->mbr.area();
      }
    }

    return this->children[idx];
  }

public:
  bool isLeaf;
  MBB mbr;
  std::vector<Point> points;
  std::vector<RNode *> children;

  RNode(bool leaf) : isLeaf(leaf) {}

  RNode *insert(const Point &p, uint maxEntries) {
    if (this->isLeaf) {
      if (this->points.size() < maxEntries) {
        this->points.push_back(p);
        if (this->mbr.isEmpty) {
          this->mbr = MBB(p, p);
          this->mbr.isEmpty = false;
        } else {
          this->mbr.expandToInclude(p);
        }
        return nullptr;
      }
      // Si el nodo hoja esta lleno, hacer un split
      this->points.push_back(p);
      RNode *newNode = linearSplitLeaf(maxEntries);
      return newNode;
    }

    // Si no es hoja, buscar el hijo mas adecuado
    RNode *bestChild = chooseSubtree(p);

    // Insertar el punto en el hijo adecuado
    RNode *newChild = bestChild->insert(p, maxEntries);

    // Si el hijo devuelve un nuevo nodo, agregarlo al nodo actual
    if (newChild != nullptr) {
      this->children.push_back(newChild);
      this->mbr.expandToInclude(newChild->mbr);
      this->mbr = MBB::computeFromNodes(this->children);

      // Verificar si el nodo actual está lleno y realizar un split si es
      // necesario
      if (this->children.size() > maxEntries) {
        return quadraticSplitInternal(maxEntries);
      }

      return nullptr;
    }
    // Si no se devuelve un nuevo nodo, expandir el MBB del nodo actual
    this->mbr.expandToInclude(p);
    this->mbr = MBB::computeFromNodes(this->children);
    return nullptr;
  };

  void balancePoints(RNode* other, uint minEntries) {
    while (this->points.size() < minEntries && !other->points.empty()) {
      Point p = other->points.back();
      this->points.push_back(p);
      other->points.pop_back();
    }
  }

  void balanceNodes(RNode* other, uint minEntries) {
    while (this->children.size() < minEntries && !other->children.empty()) {
      RNode *child = other->children.back();
      this->children.push_back(child);
      other->children.pop_back();
    }
  }

  void search(const MBB &query, std::vector<Point>& res) const {
    const float tol = 1e-6f;

    if (this->isLeaf) {
      for (const Point &p : this->points) {
        if (query.distanceTo(p) < tol) {
          res.push_back(p);
        }
      }
    } else {
      for (RNode *child : this->children) {
        if (query.intersects(child->mbr) > -tol) {
          child->search(query, res);
        }
      }
    }
  };
};

inline MBB MBB::computeFromNodes(const std::vector<RNode *> &nodes) {
  if (nodes.empty())
    return MBB();

  RNode *firstNode = nodes[0];
  MBB res(firstNode->mbr.lower, firstNode->mbr.upper);

  for (size_t i = 1; i < nodes.size(); ++i) {
    res.expandToInclude(nodes[i]->mbr);
  }

  return res;
};

// -------------------------------
// Para Best-First
// -------------------------------
struct QueueEntry {
  float distance; // Distancia desde el query al MBB
  bool isNode;    // Si true, es un nodo; si false, es un punto
  RNode *node;
  Point pt;
};

struct QueueEntryComparator {
  bool operator()(const QueueEntry &a, const QueueEntry &b) const {
    return a.distance > b.distance;
  }
};

// -------------------------------
// Clase RTree
// -------------------------------
class RTree {
public:
  RNode *root;
  uint maxEntries; // Capacidad maxima

  RTree(uint maxEntries = 3) : maxEntries(maxEntries) {
    root = new RNode(true);
  }

  void insert(const Point &p) {
    RNode *child = root->insert(p, maxEntries);
    if (child != nullptr) {
      // Si la raíz está llena, crea un nuevo nodo raíz
      RNode *newRoot = new RNode(false);
      newRoot->children.push_back(root);
      newRoot->children.push_back(child);
      newRoot->mbr = MBB::unionOf(root->mbr, child->mbr);
      root = newRoot;
    }
  };

  std::vector<Point> search(const MBB &query) const {
    std::vector<Point> res;
    root->search(query, res);
    return res;
  };

  std::vector<Point> kNN(const Point &query, uint k) const {
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryComparator> searchQueue;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryComparator> res;

    QueueEntry first;
    first.distance = root->mbr.distanceTo(query);
    first.isNode = true;
    first.node = root;
    first.pt = query;

    searchQueue.push(first);

    while (!searchQueue.empty()) {
      QueueEntry curr = searchQueue.top();
      searchQueue.pop();

      if (res.size() >= k && curr.distance > res.top().distance) {
        break;
      }

      QueueEntry newEntry;

      if (curr.isNode) {
        RNode *node = curr.node;
        if (node->isLeaf) {
          for (int i = 0; i < node->points.size(); ++i) {
            newEntry.distance = query.distanceTo(node->points[i]);
            newEntry.isNode = false;
            newEntry.node = nullptr;
            newEntry.pt = node->points[i];
            searchQueue.push(newEntry);
          }
        } else {
          for (int i = 0; i < node->children.size(); ++i) {
            newEntry.distance = node->children[i]->mbr.distanceTo(query);
            newEntry.isNode = true;
            newEntry.node = node->children[i];
            newEntry.pt = Point();
            searchQueue.push(newEntry);
          }
        }
      } else {
        float dist = query.distanceTo(curr.pt);
        newEntry.distance = dist;
        newEntry.isNode = false;
        newEntry.node = nullptr;
        newEntry.pt = curr.pt;
        if (res.size() < k) {
          res.push(newEntry);
        } else if (dist < res.top().distance) {
          res.pop();
          res.push(newEntry);
        }
      }
    }

    std::vector<Point> knn;
    while (!res.empty()) {
      knn.push_back(res.top().pt);
      res.pop();
    }

    return knn;
  };
};
