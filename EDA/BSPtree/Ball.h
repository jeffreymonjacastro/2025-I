#ifndef BALL_H
#define BALL_H

#include "Point.h"
#include "Line.h"
#include "DataType.h"

template <typename T = NType>
class Ball {
private:
    Point3D<T>  position_;
    Vector3D<T> velocity_;
    T radius_;

public:
    // Constructores
    Ball() : position_(), velocity_(), radius_(static_cast<T>(1)) {}
    Ball(const Point3D<T>& pos, const Vector3D<T>& vel, const T& radius)
        : position_(pos), velocity_(vel), radius_(radius) {}

    // Getters
    inline const Point3D<T>&  getPosition() const { return position_; }
    inline const Vector3D<T>& getVelocity() const { return velocity_; }
    inline const T& getRadius() const { return radius_; }

    // Setters
    inline void setPosition(const Point3D<T>& pos) { position_ = pos; }
    inline void setVelocity(const Vector3D<T>& vel) { velocity_ = vel; }
    inline void setRadius  (const T& r) { radius_ = r; }

    LineSegment<T> step(const T& dt) {
        Point3D<T> oldPosition = position_;
        position_ = position_ + (velocity_ * dt);
        return LineSegment<T>(oldPosition, position_);
    }
};

#endif // BALL_H
