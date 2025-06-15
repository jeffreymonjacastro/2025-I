#ifndef MTREE_H
#define MTREE_H

#include <vector>
#include <cstddef>
#include "Object.h"

class MNode {
public:
    explicit MNode(bool leaf, MNode* parent = nullptr)
      : _parent(parent),
        _isLeaf(leaf),
        _pivot(nullptr),
        _parentDistance(0),
        _radius(0)
    {}
    explicit MNode(MNode* parent, bool leaf, Object* pivot, std::size_t parentDistance, std::size_t radius)
      : _parent(parent),
        _isLeaf(leaf),
        _pivot(pivot),
        _parentDistance(parentDistance),
        _radius(radius)
    {}

    ~MNode() {
        for (MNode* child : _children)
            delete child;
    }

    // Getters
    bool isLeaf() const noexcept { return _isLeaf; }
    MNode* parent() const noexcept { return _parent; }
    Object* pivot() const noexcept { return _pivot; }
    std::size_t parentDistance() const noexcept { return _parentDistance; }
    std::size_t radius() const noexcept { return _radius; }
    const std::vector<Object*>& objects() const noexcept { return _objects; }
    const std::vector<MNode*>& children() const noexcept { return _children; }
    const std::vector<std::size_t>& pivotDistances() const noexcept { return _pivotDist; }

    std::size_t size() const noexcept { return _isLeaf ? _objects.size() : _children.size(); }
    
    // Inserción
    bool insert(const Object& obj, std::size_t maxEntries);

    // Busqueda por radio
    void rangeSearch(const Object& query, std::size_t radius, std::vector<Object*>& out) const;

private:
    MNode* _parent;     // Nodo padre
    bool   _isLeaf;     // Es un nodo hoja?

    Object*                 _pivot;     // Pivote del nodo
    std::size_t    _parentDistance;     // Distancia al pivote del padre
    std::size_t            _radius;     // Radio de cobertura del nodo

    // Datos del nodo
    std::vector<Object*>  _objects; // Nodo hoja: objetos
    std::vector<MNode*>  _children; // Nodo interior: nodos hijos

    // Distancia de los hijos/objetos al pivote
    std::vector<std::size_t> _pivotDist;  // d(obj[i], pivot) o d(children[i], pivot)

};


class MTree {
private:
    MNode*            _root;
    std::size_t _maxEntries;

public:
    explicit MTree(std::size_t maxEntries = 10)
      : _root(nullptr), _maxEntries(maxEntries) {}

    ~MTree() {
        delete _root;
    }

    // Getters
    MNode* root() const noexcept { return _root; }
    std::size_t maxEntries() const noexcept { return _maxEntries; }

    void insert(const Object& obj);
    bool search(const Object& obj) const;

    // Búsqueda por radio
    std::vector<Object*> rangeSearch(const Object& query, std::size_t radius) const;

    // k-nearest neighbors search
    std::vector<Object*> kNearestNeighbors(const Object& query, std::size_t k) const;
};

#endif // MTREE_H