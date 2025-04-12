#ifndef PLANE_H
#define PLANE_H

#include "DataType.h"
#include "Point.h"
#include "Line.h"
#include <vector>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <cmath>

// Forward declarations
template <typename T>
class Plane;

template <typename T>
class Polygon;

// Plane class template
template <typename T = NType>
class Plane {
private:
    Point3D <T> point_;    // A point on the plane
    Vector3D<T> normal_;    // A normal vector to the plane

public:
    // Constructors
    Plane() : point_(), 
              normal_(Vector3D<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1))) {}
    Plane(const Point3D<T>& point, const Vector3D<T>& normal) : point_(point), normal_(normal.normalized()) {}

    // Distance from a point to the plane
    T distance(const Point3D<T>& p) const;

    // Intersection with a line
    Point3D<T> intersect(const Line<T>& l) const;

    // Containment checks
    bool contains(const Point3D<T>& p) const;
    bool contains(const Line<T>& l) const;

    // Getters
    Point3D<T> getPoint() const { return point_; }
    Vector3D<T> getNormal() const { return normal_; }

    // Setters
    void setPoint(const Point3D<T>& point) { point_ = point; }
    void setNormal(const Vector3D<T>& normal) { normal_ = normal.normalized(); }

    // Operators
    bool operator==(const Plane& other) const;
    bool operator!=(const Plane& other) const { return !(*this == other); }

    // Output operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Plane<U>& plane);
};

// Polygon class template
template <typename T = NType>
class Polygon {
private:
    std::vector<Point3D<T>> vertices_;

public:
    // Constructors
    Polygon() : vertices_() {}
    Polygon(const std::vector<Point3D<T>>& vertices) : vertices_(vertices) {}

    // Getters
    std::vector<Point3D<T>> getVertices() const { return vertices_; }
    const Point3D<T>& getVertex(size_t index) const { return vertices_.at(index); }
    Plane<T>    getPlane   () const;    // Get the plane    of the polygon
    Vector3D<T> getNormal  () const;    // Get the normal   of the polygon
    Point3D<T>  getCentroid() const;    // Get the centroid of the polygon

    // Setters
    void setVertices(const std::vector<Point3D<T>>& vertices) { vertices_ = vertices; }

    // Check if a point is inside the polygon
    bool contains(const Point3D<T>& p) const;

    // Get the relation of the polygon with a plane
    RelationType relationWithPlane(const Plane<T>& plane) const;

    // Split the polygon by a plane
    std::pair<Polygon<T>, Polygon<T>> split(const Plane<T>& plane) const;

    // Compute the area of the polygon
    T area() const;

    // Operators
    bool operator==(const Polygon& other) const;
    bool operator!=(const Polygon& other) const { return !(*this == other); }

    // Output operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Polygon<U>& polygon);
};

// Implementations
// Plane
template <typename T>
T Plane<T>::distance(const Point3D<T>& p) const {
    return abs(normal_.dot(p - point_));
}

template <typename T>
Point3D<T> Plane<T>::intersect(const Line<T>& l) const {
    try {
        if (this->contains(l)) {
            throw std::runtime_error("Line is contained in the plane");
        }

        Point3D<T> P = l.getPoint();
        Vector3D<T> v = l.getDirection();

        T t = -normal_.dot(P - point_) / normal_.dot(v);

        if (t >= static_cast<T>(0)) {
            Point3D<T> point_v(v.getX(), v.getY(), v.getZ());
            return P + point_v * t;
        }

        throw std::runtime_error("No intersection with the plane");
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";

        return Point3D<T>(T(0), T(0), T(0));
    }
}

template <typename T>
bool Plane<T>::contains(const Point3D<T>& p) const {
    Point3D<T> P = p - point_;
    T dot = normal_.dot(P);

    if (dot == static_cast<T>(0)) {
        return true;
    }

    return false;
}

template <typename T>
bool Plane<T>::contains(const Line<T>& l) const {
    Vector3D<T> v = l.getDirection();
    Point3D<T> point_v(v.getX(), v.getY(), v.getZ());
    T dot = normal_.dot(point_v);

    if (dot == static_cast<T>(0)) {
        return true;
    }

    return false;
}

// Equality operators
template <typename T>
bool Plane<T>::operator==(const Plane<T>& other) const {
    bool normalsEqual = (normal_ == other.normal_) || (normal_ == -other.normal_);
    return normalsEqual && contains(other.point_);
}

// Output operator for Plane
template <typename T>
std::ostream& operator<<(std::ostream& os, const Plane<T>& plane) {
    os << "Point: " << plane.point_ << ", Normal: " << plane.normal_;
    return os;
}

// Polygon

// Equality operators
template <typename T>
bool Polygon<T>::operator==(const Polygon<T>& other) const {
    return vertices_ == other.vertices_;
}

template <typename T>
Plane<T> Polygon<T>::getPlane() const {
    return Plane<T>();
}

template <typename T>
Vector3D <T> Polygon<T>::getNormal() const {
    return Vector3D<T>();
}

template <typename T>
Point3D<T> Polygon<T>::getCentroid() const {
    return Point3D<T>();
}

template <typename T>
bool Polygon<T>::contains(const Point3D<T>& p) const {
    p.getX();
    return bool();
}

template <typename T>
RelationType Polygon<T>::relationWithPlane(const Plane<T>& plane) const {
    plane = plane.getNormal();
    return COINCIDENT;
}

template <typename T>
std::pair<Polygon<T>, Polygon<T>> Polygon<T>::split(const Plane<T>& plane) const {
    plane = plane.getNormal();
    return std::make_pair(Polygon<T>(), Polygon<T>());
}

template <typename T>
T Polygon<T>::area() const {
    return T();
}

// Output operator for Polygon
template <typename T>
std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon) {
    os << "Vertices: ";
    for (const auto& vertex : polygon.vertices_) {
        os << vertex << " ";
    }
    return os;
}

#endif // PLANE_H