#ifndef MTREE_H
#define MTREE_H

#include "Object.h"
#include <cstddef>
#include <vector>

class MNode {
private:
  MNode *_parent; // Nodo padre
  bool _isLeaf;   // Es un nodo hoja?

  Object *_pivot;         // Pivote del nodo
  size_t _parentDistance; // Distancia al pivote del padre
  size_t _radius;         // Radio de cobertura del nodo

  // Datos del nodo
  vector<Object *> _objects; // Nodo hoja: objetos
  vector<MNode *> _children; // Nodo interior: nodos hijos

  // Distancia de los hijos/objetos al pivote
  vector<size_t> _pivotDist; // d(obj[i], pivot) o d(children[i], pivot)

public:
  explicit MNode(bool leaf, MNode *parent = nullptr)
      : _parent(parent),
        _isLeaf(leaf),
        _pivot(nullptr),
        _parentDistance(0),
        _radius(0) {}
  explicit MNode(MNode *parent, bool leaf, Object *pivot, size_t parentDistance, size_t radius)
      : _parent(parent),
        _isLeaf(leaf),
        _pivot(pivot),
        _parentDistance(parentDistance),
        _radius(radius) {}

  ~MNode() {
    for (MNode *child : _children)
      delete child;
  }

  // Getters
  bool isLeaf() const noexcept { return _isLeaf; }
  MNode *parent() const noexcept { return _parent; }
  Object *pivot() const noexcept { return _pivot; }
  size_t parentDistance() const noexcept { return _parentDistance; }
  size_t radius() const noexcept { return _radius; }
  const vector<Object *> &objects() const noexcept { return _objects; }
  const vector<MNode *> &children() const noexcept { return _children; }
  const vector<size_t> &pivotDistances() const noexcept { return _pivotDist; }

  size_t size() const noexcept { return _isLeaf ? _objects.size() : _children.size(); }

  // Inserción
  bool insert(const Object &obj, size_t maxEntries);

  // Busqueda por radio
  void rangeSearch(const Object &query, size_t radius, vector<Object *> &out) const;
};

class MTree {
private:
  MNode *_root;
  size_t _maxEntries;

public:
  explicit MTree(size_t maxEntries = 10) : _root(nullptr), _maxEntries(maxEntries) {}

  ~MTree() {
    delete _root;
  }

  // Getters
  MNode *root() const noexcept { return _root; }
  size_t maxEntries() const noexcept { return _maxEntries; }

  void insert(const Object &obj);
  bool search(const Object &obj) const;

  // Búsqueda por radio
  vector<Object *> rangeSearch(const Object &query, size_t radius) const;

  // k-nearest neighbors search
  vector<Object *> kNearestNeighbors(const Object &query, size_t k) const;
};

// ============================ MNode ===========================================
bool MNode::insert(const Object &obj, size_t maxEntries) {
  return false;
}

void MNode::rangeSearch(const Object &query, size_t radius, vector<Object *> &out) const {}

// ============================ MTree ===========================================
void MTree::insert(const Object &ob) {}

bool MTree::search(const Object &obj) const {
  return false;
}

vector<Object *> MTree::rangeSearch(const Object &query, size_t radius) const {
  vector<Object *> results;
  if (_root) {
    _root->rangeSearch(query, radius, results);
  }
  return results;
}

vector<Object *> MTree::kNearestNeighbors(const Object &query, size_t k) const {
  vector<Object *> results;
  if (_root) {
    // Implementación de kNN
    // Aquí se debería implementar la lógica para encontrar los k vecinos más cercanos
  }
  return results;
}

#endif // MTREE_H