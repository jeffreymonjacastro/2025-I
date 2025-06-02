#ifndef SPHERE_H
#define SPHERE_H

#include "Point.h"

struct Sphere {
    Point center;
    float radius;

    Sphere() 
      : center(), radius(0.0f) {}

    Sphere(const Point& c, float r)
      : center(c), radius(r) {}

    void expandToInclude(const Sphere& other);
    void expandToInclude(const Point& p);
};

#endif // SPHERE_H
