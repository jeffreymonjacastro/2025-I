#ifndef POINT_H
#define POINT_H

#include "DataType.h"
#include <iostream>
#include <stdexcept>

template <typename T = NType>
class Point3D {
private:
    T x_, y_, z_;

public:
    // Constructors
    Point3D() : x_(static_cast<T>(0)), y_(static_cast<T>(0)), z_(static_cast<T>(0)) {}
    Point3D(const T& x, const T& y, const T& z) : x_(x), y_(y), z_(z) {}

    // Getters
    inline const T& getX() const { return x_; }
    inline const T& getY() const { return y_; }
    inline const T& getZ() const { return z_; }

    // Setters
    inline void setX(const T& x) { x_ = x; }
    inline void setY(const T& y) { y_ = y; }
    inline void setZ(const T& z) { z_ = z; }

    // Operator Overloads
    bool operator==(const Point3D& p) const {
        return x_ == p.x_ && y_ == p.y_ && z_ == p.z_;
    }
    bool operator!=(const Point3D& p) const {
        return !(*this == p);
    }

    Point3D operator+(const Point3D& p) const {
        return Point3D(x_ + p.x_, y_ + p.y_, z_ + p.z_);
    }
    Point3D operator-(const Point3D& p) const {
        return Point3D(x_ - p.x_, y_ - p.y_, z_ - p.z_);
    }
    Point3D operator*(const T& scalar) const {
        return Point3D(x_ * scalar, y_ * scalar, z_ * scalar);
    }
    Point3D operator/(const T& scalar) const {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Division by zero");
        }
        return Point3D(x_ / scalar, y_ / scalar, z_ / scalar);
    }

    // Compound Assignment Operators
    Point3D& operator+=(const Point3D& p) {
        x_ += p.x_;
        y_ += p.y_;
        z_ += p.z_;
        return *this;
    }
    Point3D& operator-=(const Point3D& p) {
        x_ -= p.x_;
        y_ -= p.y_;
        z_ -= p.z_;
        return *this;
    }
    Point3D& operator*=(const T& scalar) {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }
    Point3D& operator/=(const T& scalar) {
        if (scalar == static_cast<T>(0)) {
            throw std::runtime_error("Division by zero");
        }
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
        return *this;
    }

    // Vector Operations
    // Dot Product
    T dot(const Point3D& p) const {
        return x_ * p.x_ + y_ * p.y_ + z_ * p.z_;
    }

    // Cross Product
    Point3D cross(const Point3D& p) const {
        return Point3D(
            y_ * p.z_ - z_ * p.y_,
            z_ * p.x_ - x_ * p.z_,
            x_ * p.y_ - y_ * p.x_
        );
    }

    // Length of the Vector
    T magnitude() const {
        return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    // Normalization
    Point3D normalized() const {
        T mag = magnitude();
        if (mag == static_cast<T>(0)) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        return *this / mag;
    }

    // Distance between two points
    T distance(const Point3D& p) const {
        return (*this - p).magnitude();
    }

    // Calculate the angle (in radians) between this vector and another vector
    T angle(const Point3D& p) const {
        T dotProd = this->dot(p);
        T magA = this->magnitude();
        T magB = p.magnitude();
        if (magA == static_cast<T>(0) || magB == static_cast<T>(0)) {
            throw std::runtime_error("Cannot calculate angle with a zero-length vector");
        }
        // Limitamos cosine al rango [-1, 1]
        T cosine = dotProd / (magA * magB);
        if (cosine < static_cast<T>(-1)) cosine = static_cast<T>(-1);
        if (cosine > static_cast<T>( 1)) cosine = static_cast<T>(1);
        return acos(cosine);
    }

    // Linear interpolation between this point and another point
    Point3D lerp(const Point3D& p, const T& t) const {
        // t: [0,1], interpolation factor
        return *this * (static_cast<T>(1) - t) + p * t;
    }

    // Stream Output Operator
    friend std::ostream& operator<<(std::ostream& os, const Point3D& p) {
        os << "(" << p.x_ << ", " << p.y_ << ", " << p.z_ << ")";
        return os;
    }
};

// Scalar Multiplication from Left
template <typename T>
Point3D<T> operator*(const T& scalar, const Point3D<T>& p) {
    return p * scalar;
}

#endif // POINT_H
