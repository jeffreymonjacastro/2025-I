#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include <iostream>
#include <stdexcept>

template <typename T>
class Line;

template <typename T>
class LineSegment;

template <typename T>
class Vector3D;

// LineSegment class template
template <typename T = NType>
class LineSegment {
private:
    Point3D<T> p1_;
    Point3D<T> p2_;

public:
    // Constructors
    LineSegment() : p1_(), p2_() {}
    LineSegment(const Point3D<T>& p1, const Point3D<T>& p2) : p1_(p1), p2_(p2) {}

    // Getters
    const Point3D<T>& getP1() const { return p1_; }
    const Point3D<T>& getP2() const { return p2_; }

    // Setters
    void setP1(const Point3D<T>& p1) { p1_ = p1; }
    void setP2(const Point3D<T>& p2) { p2_ = p2; }

    // Length of the segment
    T length() const {
        return p1_.distance(p2_);
    }

    // Equality operators
    bool operator==(const LineSegment& l) const {
        return p1_ == l.p1_ && p2_ == l.p2_;
    }
    bool operator!=(const LineSegment& l) const {
        return !(*this == l);
    }

    // Line containing the segment
    Line<T> getLine() const;

    // Output operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const LineSegment<U>& l);
};

// Vector3D class template
template <typename T = NType>
class Vector3D : public Point3D<T> {
public:
    // Constructors
    Vector3D() : Point3D<T>() {}
    Vector3D(const T& x, const T& y, const T& z) : Point3D<T>(x, y, z) {}
    Vector3D(const Point3D<T>& p) : Point3D<T>(p) {}
    ~Vector3D() = default;

    // Dot product
    T dotProduct(const Vector3D& v) const {
        return this->getX() * v.getX() + this->getY() * v.getY() + this->getZ() * v.getZ();
    }

    // Cross product
    Vector3D crossProduct(const Vector3D& v) const {
        return Vector3D(
            this->getY() * v.getZ() - this->getZ() * v.getY(),
            this->getZ() * v.getX() - this->getX() * v.getZ(),
            this->getX() * v.getY() - this->getY() * v.getX()
        );
    }

    // Magnitude
    T magnitude() const {
        return Point3D<T>::magnitude();
    }

    // Unit vector
    Vector3D unit() const {
        T mag = magnitude();
        if (mag == static_cast<T>(0)) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        return *this / mag;
    }

    // Normalize the vector
    void normalize() {
        T mag = magnitude();
        if (mag == static_cast<T>(0)) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        *this /= mag;
    }

    Vector3D operator-() const {
        return Vector3D(-this->getX(), -this->getY(), -this->getZ());
    }

    // Output operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Vector3D<U>& v);
};

// Line class template
template <typename T = NType>
class Line {
private:
    Point3D <T> p_; 
    Vector3D<T> v_; 

public:
    // Constructors
    Line() : p_(), v_() {}
    Line(const Point3D<T>& p, const Vector3D<T>& v) : p_(p), v_(v.unit()) {}
    Line(const Point3D<T>& p1, const Point3D<T>& p2) : p_(p1), v_(Vector3D<T>(p2 - p1).unit()) {}
    Line(const LineSegment<T>& l) : p_(l.getP1()), v_(Vector3D<T>(l.getP2() - l.getP1()).unit()) {}
    ~Line() = default;

    // Getters
    Point3D<T> getPoint() const { return p_; }
    Vector3D<T> getDirection() const { return v_; }

    // Setters
    void setPoint(const Point3D<T>& p) { p_ = p; }
    void setDirection(const Vector3D<T>& v) { v_ = v.unit(); }

    // Parallelism checks
    bool isParallel(const Line& l) const;
    bool isParallel(const Vector3D<T>& v) const;
    bool isParallel(const LineSegment<T>& l) const;

    // Orthogonality checks
    bool isOrthogonal(const Line& l) const;
    bool isOrthogonal(const Vector3D<T>& v) const;
    bool isOrthogonal(const LineSegment<T>& l) const;

    // Output operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Line<U>& l);
};

// Implementations

// LineSegment<T>::getLine()
template <typename T>
Line<T> LineSegment<T>::getLine() const {
    return Line<T>(p1_, p2_);
}


// Output operator for LineSegment
template <typename T>
std::ostream& operator<<(std::ostream& os, const LineSegment<T>& l) {
    os << "[" << l.p1_ << " to " << l.p2_ << "]";
    return os;
}

// Output operator for Vector3D
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v) {
    os << "{" << v.getX() << ", " << v.getY() << ", " << v.getZ() << "}";
    return os;
}

// Output operator for Line
template <typename T>
std::ostream& operator<<(std::ostream& os, const Line<T>& l) {
    os << "Point: " << l.p_ << ", Direction: " << l.v_;
    return os;
}

// Line<T>::isParallel methods
template <typename T>
bool Line<T>::isParallel(const Line<T>& l) const {
    T crossMag = v_.crossProduct(l.v_).magnitude();
    return crossMag == static_cast<T>(0);
}

template <typename T>
bool Line<T>::isParallel(const Vector3D<T>& v) const {
    T crossMag = v_.crossProduct(v).magnitude();
    return crossMag == static_cast<T>(0);
}

template <typename T>
bool Line<T>::isParallel(const LineSegment<T>& l) const {
    Vector3D<T> dir = Vector3D<T>(l.getP2() - l.getP1()).unit();
    T crossMag = v_.crossProduct(dir).magnitude();
    return crossMag == static_cast<T>(0);
}

// Line<T>::isOrthogonal methods
template <typename T>
bool Line<T>::isOrthogonal(const Line<T>& l) const {
    T dotProd = v_.dotProduct(l.v_);
    return dotProd == static_cast<T>(0);
}

template <typename T>
bool Line<T>::isOrthogonal(const Vector3D<T>& v) const {
    T dotProd = v_.dotProduct(v);
    return dotProd == static_cast<T>(0);
}

template <typename T>
bool Line<T>::isOrthogonal(const LineSegment<T>& l) const {
    Vector3D<T> dir = Vector3D<T>(l.getP2() - l.getP1()).unit();
    T dotProd = v_.dotProduct(dir);
    return dotProd == static_cast<T>(0);
}

#endif // LINE_H
