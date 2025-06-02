#ifndef SRTREE_H
#define SRTREE_H

#include <vector>
#include <limits>
#include <algorithm>
#include <string>
#include <cmath>
#include <stdexcept>
#include "Point.h"
#include "MBB.h"
#include "Sphere.h"


class SRNode {
private:
    MBB       _boundingBox;
    Sphere _boundingSphere;
    SRNode*        _parent;
    std::vector<Point*>    _points;
    std::vector<SRNode*> _children;
    bool _isLeaf;

public:
    bool    getIsLeaf() const { return _isLeaf; }
    SRNode* getParent() const { return _parent; }

    const    MBB& getBoundingBox   () const { return    _boundingBox; }
    const Sphere& getBoundingSphere() const { return _boundingSphere; }
    const std::vector< Point*>&   getPoints() const { return   _points; }
    const std::vector<SRNode*>& getChildren() const { return _children; }
    std::size_t getNumPoints  () const { return   _points.size(); }
    std::size_t getNumChildren() const { return _children.size(); }
    bool getIsLeaf() const { return _isLeaf; }

    void setBoundingBox   (const    MBB&    box) { _boundingBox    =    box; }
    void setBoundingSphere(const Sphere& sphere) { _boundingSphere = sphere; }
    void setParent        (      SRNode* parent) {         _parent = parent; }
    void setIsLeaf        (bool          isLeaf) {         _isLeaf = isLeaf; }

    // Insert algorithm
    SRNode* insert(Point& _data, std::size_t maxEntries);

};


class SRTree {
private:
    SRNode*           _root;
    std::size_t _maxEntries;
    
public:

    SRTree() : _maxEntries(15), _root(nullptr) {}
    explicit SRTree(std::size_t maxEntries) : _maxEntries(maxEntries), _root(nullptr) {}

    SRNode* getRoot() const { return _root; }

    void insert(const Point& point);
    bool search(const Point& point) const;
    std::vector<Point*> rangeQuery(const MBB& box) const;
    std::vector<Point*> rangeQuery(const Sphere& sphere) const;

    // k-nearest neighbors search
    std::vector<Point*> kNearestNeighbors(const Point& point, std::size_t k) const;
};

#endif // SRTREE_H