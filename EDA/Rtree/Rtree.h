#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

typedef unsigned int  uint;
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
    Point lower; // Esquina inferior izquierda
    Point upper; // Esquina superior derecha

    MBB() : lower(Point()), upper(Point()) {}
    MBB(const Point &p1, const Point &p2)
        : lower(Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y))),
          upper(Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y))) {}

    float area() const;
    float semiPerimeter() const;

    float distanceTo(const Point &p) const;

    // Incremento del semiperimetro si agrega p
    float deltaSemiPerimeter(const Point &p) const;

    // Expande para incluir point o MBB
    void expandToInclude(const Point &p);
    void expandToInclude(const MBB &other);

    // Retorna el area de interseccion
    float intersects(const MBB &other) const;
    
    
    // Crear MBB a partir de un vector de puntos
    static MBB computeFromPoints(const std::vector<Point> &pts);

    // Crear MBB a partir de un vector de nodos
    static MBB computeFromNodes(const std::vector<RNode*> &nodes);

    // Union MBBs
    static MBB unionOf(const MBB &a, const MBB &b);
};


// -------------------------------
// Clase RNode
// -------------------------------
class RNode {
private:
    // Linear Split para nodos hojas
    RNode* linearSplitLeaf(uchar maxEntries);

    // Quadratic Split para nodos internos
    RNode* quadraticSplitInternal(uchar maxEntries);

public:
    bool isLeaf;
    MBB mbr;
    std::vector<Point>    points;
    std::vector<RNode*> children;

    RNode(bool leaf) : isLeaf(leaf) {}

    RNode* insert(const Point &p, uchar maxEntries);
    std::vector<Point> search(const MBB &query) const;
};



// -------------------------------
// Para Best-First
// -------------------------------
struct QueueEntry {
    float distance;  // Distancia desde el query al MBB
    bool isNode;     // Si true, es un nodo; si false, es un punto
    RNode* node;
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
    RNode* root;
    uchar maxEntries;  // Capacidad maxima

    RTree(uchar maxEntries = 3) : maxEntries(maxEntries) {
        root = new RNode(true);
    }

    void insert(const Point &p);
    std::vector<Point> search(const MBB &query) const;
    std::vector<Point> kNN(const Point &query, uchar k) const;
};
