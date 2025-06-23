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

  pair<Object *, Object *> choosePivots(const vector<Object *> &allObjects);
  pair<vector<Object *>, vector<Object *>> partition(const vector<Object *> &objects, Object *p1, Object *p2, size_t maxEntries);
  MNode *split(Object *newPivot, size_t maxEntries);
  MNode *chooseSubtree(const Object &obj);

  // Inserción
  bool insert(Object *obj, size_t maxEntries);

  // Busqueda por radio
  void rangeSearch(const Object &query, size_t radius, vector<Object *> &out) const;

  void kNNSearch(const Object &query, size_t k,
                 vector<pair<size_t, Object *>> &candidates,
                 size_t &maxDist) const;
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

  void insert(Object *obj);
  bool search(const Object &obj) const;

  // Búsqueda por radio
  vector<Object *> rangeSearch(const Object &query, size_t radius) const;

  // k-nearest neighbors search
  vector<Object *> kNearestNeighbors(const Object &query, size_t k) const;
};

// ============================ MNode ===========================================
pair<Object *, Object *> MNode::choosePivots(const vector<Object *> &allObjects) {
  if (allObjects.size() < 2) {
    return {nullptr, nullptr};
  }

  size_t maxDist = 0;
  Object *p1 = allObjects[0];
  Object *p2 = allObjects[1];

  for (size_t i = 0; i < allObjects.size(); ++i) {
    for (size_t j = i + 1; j < allObjects.size(); ++j) {
      size_t dist = allObjects[i]->distance(allObjects[j]);
      if (dist > maxDist) {
        maxDist = dist;
        p1 = allObjects[i];
        p2 = allObjects[j];
      }
    }
  }

  return {p1, p2};
}

pair<vector<Object *>, vector<Object *>> MNode::partition(const vector<Object *> &objects, Object *p1, Object *p2, size_t maxEntries) {
  vector<Object *> s1, s2;

  for (Object *obj : objects) {
    size_t distP1 = obj->distance(p1);
    size_t distP2 = obj->distance(p2);

    if (distP1 <= distP2) {
      if (s1.size() > maxEntries / 2)
        s2.push_back(obj);
      else
        s1.push_back(obj);
    } else {
      if (s2.size() > maxEntries / 2)
        s1.push_back(obj);
      else
        s2.push_back(obj);
    }
  }

  return {s1, s2};
}

MNode *MNode::split(Object *newPivot, size_t maxEntries) {
  vector<Object *> allObjects;
  vector<MNode *> originalChildren;

  if (_isLeaf) {
    allObjects = _objects;
  } else {
    originalChildren = _children;
    for (MNode *child : originalChildren) {
      allObjects.push_back(child->pivot());
    }
  }

  auto [p1, p2] = choosePivots(allObjects);
  auto [s1, s2] = partition(allObjects, p1, p2, maxEntries);

  MNode *newNode = new MNode(_isLeaf, _parent);

  _objects.clear();
  _children.clear();
  _pivotDist.clear();

  Object *oldPivot = _pivot;

  _pivot = p1;
  _radius = 0;

  newNode->_pivot = p2;
  newNode->_radius = 0;

  if (_isLeaf) {
    for (Object *obj : s1) {
      _objects.push_back(obj);
      size_t dist = _pivot->distance(obj);
      _pivotDist.push_back(dist);
      if (dist > _radius) {
        _radius = dist;
      }
    }

    for (Object *obj : s2) {
      newNode->_objects.push_back(obj);
      size_t dist = newNode->_pivot->distance(obj);
      newNode->_pivotDist.push_back(dist);
      if (dist > newNode->_radius) {
        newNode->_radius = dist;
      }
    }
  } else {
    for (Object *pivotObj : s1) {
      for (MNode *child : originalChildren) {
        if (child->pivot() == pivotObj) {
          _children.push_back(child);
          child->_parent = this;
          size_t dist = _pivot->distance(pivotObj);
          _pivotDist.push_back(dist);

          child->_parentDistance = dist;

          size_t requiredRadius = dist + child->radius();
          if (requiredRadius > _radius) {
            _radius = requiredRadius;
          }
          break;
        }
      }
    }

    for (Object *pivotObj : s2) {
      for (MNode *child : originalChildren) {
        if (child->pivot() == pivotObj) {
          newNode->_children.push_back(child);
          child->_parent = newNode;
          size_t dist = newNode->_pivot->distance(pivotObj);
          newNode->_pivotDist.push_back(dist);

          child->_parentDistance = dist;

          size_t requiredRadius = dist + child->radius();
          if (requiredRadius > newNode->_radius) {
            newNode->_radius = requiredRadius;
          }
          break;
        }
      }
    }
  }

  if (_parent && oldPivot != _pivot) {
    for (size_t i = 0; i < _parent->_children.size(); i++) {
      if (_parent->_children[i] == this) {
        _parent->_pivotDist[i] = _parent->_pivot->distance(_pivot);
        _parentDistance = _parent->_pivotDist[i];
        break;
      }
    }
  }

  return newNode;
}

MNode *MNode::chooseSubtree(const Object &obj) {
  if (children().empty()) {
    return nullptr;
  }

  vector<size_t> candidates;

  for (size_t i = 0; i < children().size(); ++i) {
    size_t dist = children()[i]->pivot()->distance(&obj);
    if (dist <= children()[i]->radius()) {
      candidates.push_back(i);
    }
  }

  size_t bestChild;

  if (candidates.empty()) {
    bestChild = 0;
    size_t min_r = SIZE_MAX;

    for (size_t i = 0; i < size(); ++i) {
      size_t dist = children()[i]->pivot()->distance(&obj);
      size_t curr_r = children()[i]->radius();

      if (dist - curr_r < min_r) {
        min_r = dist - curr_r;
        bestChild = i;
      }
    }

    return children()[bestChild];
  }

  bestChild = candidates[0];
  size_t minDist = children()[bestChild]->pivot()->distance(&obj);

  for (size_t i = 1; i < candidates.size(); ++i) {
    size_t candidate = candidates[i];
    size_t dist = children()[candidate]->pivot()->distance(&obj);

    if (dist < minDist) {
      minDist = dist;
      bestChild = candidate;
    }
  }

  return children()[bestChild];
}

bool MNode::insert(Object *obj, size_t maxEntries) {
  if (isLeaf()) {
    if (!pivot()) {
      this->_pivot = obj;
      this->_objects.push_back(_pivot);
      this->_pivotDist.push_back(0);
      this->_radius = 0;
      return false;
    }

    Object *newObj = obj;
    this->_objects.push_back(newObj);

    size_t dist = _pivot->distance(newObj);
    this->_pivotDist.push_back(dist);

    if (dist > radius()) {
      this->_radius = dist;
    }

    if (size() > maxEntries) {
      MNode *newNode = split(newObj, maxEntries);

      if (parent()) {
        _parent->_children.push_back(newNode);
        newNode->_parent = _parent;
        size_t dist = _parent->_pivot->distance(newNode->_pivot);
        _parent->_pivotDist.push_back(dist);
        newNode->_parentDistance = dist;
        size_t requiredRadius = dist + newNode->_radius;

        if (requiredRadius > _parent->_radius) {
          _parent->_radius = requiredRadius;
        }

        if (_parent->size() > maxEntries) {
          MNode *newParentNode = _parent->split(nullptr, maxEntries);

          if (_parent->parent()) {
            _parent->parent()->_children.push_back(newParentNode);
            newParentNode->_parent = _parent->parent();
            size_t dist = _parent->parent()->_pivot->distance(newParentNode->_pivot);
            _parent->parent()->_pivotDist.push_back(dist);
            newParentNode->_parentDistance = dist;

            size_t requiredRadius = dist + newParentNode->_radius;
            if (requiredRadius > _parent->parent()->_radius) {
              _parent->parent()->_radius = requiredRadius;
            }
            return false;
          } else {
            MNode *newRoot = new MNode(false);
            newRoot->_pivot = _parent->_pivot;
            newRoot->_children.push_back(_parent);
            newRoot->_children.push_back(newParentNode);
            newRoot->_pivotDist.push_back(0);
            newRoot->_pivotDist.push_back(_parent->_pivot->distance(newParentNode->_pivot));

            _parent->_parentDistance = 0;
            newParentNode->_parentDistance = newRoot->_pivotDist[1];

            size_t radius1 = _parent->_radius;
            size_t radius2 = newRoot->_pivotDist[1] + newParentNode->_radius;
            newRoot->_radius = max(radius1, radius2);

            _parent->_parent = newRoot;
            newParentNode->_parent = newRoot;
            return true;
          }
        }

        return false;
      }

      MNode *newRoot = new MNode(false);
      newRoot->_pivot = _pivot;
      newRoot->_children.push_back(this);
      newRoot->_children.push_back(newNode);
      newRoot->_pivotDist.push_back(0);
      newRoot->_pivotDist.push_back(_pivot->distance(newNode->_pivot));

      this->_parentDistance = 0;
      newNode->_parentDistance = newRoot->_pivotDist[1];

      size_t radius1 = _radius;
      size_t radius2 = newRoot->_pivotDist[1] + newNode->_radius;
      newRoot->_radius = max(radius1, radius2);

      _parent = newRoot;
      newNode->_parent = newRoot;
      return true;
    }

    return false;
  }

  MNode *bestChild = chooseSubtree(*obj);

  if (!bestChild) {
    return false;
  }

  bool rootChanged = bestChild->insert(obj, maxEntries);

  size_t bestChildIndex = 0;
  for (size_t i = 0; i < size(); ++i) {
    if (_children[i] == bestChild) {
      bestChildIndex = i;
      break;
    }
  }

  size_t childDist = _pivotDist[bestChildIndex];
  size_t new_r = bestChild->radius();

  if (childDist + new_r > _radius) {
    _radius = childDist + new_r;
  }

  return rootChanged;
}

void MNode::rangeSearch(const Object &query, size_t radius, vector<Object *> &out) const {
  if (!_pivot)
    return;

  size_t pivotDist = _pivot->distance(&query);

  if (pivotDist > radius + _radius)
    return;

  if (_isLeaf) {
    for (size_t i = 0; i < _objects.size(); i++) {
      if (_objects[i]->distance(&query) <= radius) {
        out.push_back(_objects[i]);
      }
    }
  } else {
    for (size_t childIdx = 0; childIdx < _children.size(); childIdx++) {
      MNode *child = _children[childIdx];
      size_t childPivotDist = child->_pivot->distance(&query);

      if (childPivotDist <= radius + child->_radius) {
        child->rangeSearch(query, radius, out);
      }
    }
  }
}

void MNode::kNNSearch(const Object &query, size_t k,
                      vector<pair<size_t, Object *>> &candidates,
                      size_t &maxDist) const {
  if (!_pivot)
    return;

  size_t pivotDist = _pivot->distance(&query);

  if (candidates.size() == k && pivotDist > _radius + maxDist) {
    return;
  }

  if (_isLeaf) {
    for (size_t i = 0; i < _objects.size(); i++) {
      size_t dist = _objects[i]->distance(&query);

      if (candidates.size() < k) {
        candidates.push_back({dist, _objects[i]});
        push_heap(candidates.begin(), candidates.end());
        if (candidates.size() == k) {
          maxDist = candidates.front().first;
        }
      } else if (dist < maxDist) {
        pop_heap(candidates.begin(), candidates.end());
        candidates.pop_back();
        candidates.push_back({dist, _objects[i]});
        push_heap(candidates.begin(), candidates.end());
        maxDist = candidates.front().first;
      }
    }
  } else {
    vector<pair<size_t, size_t>> childrenWithMinDist;

    for (size_t i = 0; i < _children.size(); i++) {
      MNode *child = _children[i];
      size_t childPivotDist = child->_pivot->distance(&query);
      size_t minPossibleDist = (childPivotDist > child->_radius) ? (childPivotDist - child->_radius) : 0;

      if (candidates.size() < k || minPossibleDist < maxDist) {
        childrenWithMinDist.push_back({minPossibleDist, i});
      }
    }

    sort(childrenWithMinDist.begin(), childrenWithMinDist.end());

    for (const auto &[minDist, idx] : childrenWithMinDist) {
      if (candidates.size() == k && minDist >= maxDist) {
        break;
      }
      _children[idx]->kNNSearch(query, k, candidates, maxDist);
    }
  }
}

// ============================ MTree ===========================================

void MTree::insert(Object *obj) {
  // std::cout << "Insertando objeto: " << obj.str() << std::endl;

  if (!_root) {
    _root = new MNode(true);
    _root->insert(obj, _maxEntries);
    return;
  }

  bool rootChanged = _root->insert(obj, _maxEntries);

  if (rootChanged) {
    _root = _root->parent();
  }
}

bool MTree::search(const Object &obj) const {
  if (!_root)
    return false;
  std::vector<Object *> res;
  _root->rangeSearch(obj, 0, res);
  return !res.empty();
}

vector<Object *> MTree::rangeSearch(const Object &query, size_t radius) const {
  vector<Object *> res;
  if (_root) {
    _root->rangeSearch(query, radius, res);
  }
  return res;
}

vector<Object *> MTree::kNearestNeighbors(const Object &query, size_t k) const {
  vector<Object *> res;
  if (!_root || k == 0) {
    return res;
  }

  vector<pair<size_t, Object *>> candidates;
  size_t maxDist = SIZE_MAX;

  _root->kNNSearch(query, k, candidates, maxDist);

  sort_heap(candidates.begin(), candidates.end());

  for (const auto &[dist, obj] : candidates) {
    res.push_back(obj);
  }

  return res;
}

#endif // MTREE_H