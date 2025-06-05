#ifndef SPHERE_H
#define SPHERE_H

#include "Point.h"

struct Sphere {
  Point center;
  float radius;
  float minradius;

  Sphere() : center(), radius(0.0f), minradius(0.0f) {}

  Sphere(const Point &c, float r, float minr) : center(c), radius(r), minradius(minr) {}

  void expandToInclude(const Sphere &other);
  void expandToInclude(const Point &p);

  Sphere &operator=(const Sphere &other);

  friend std::ostream &operator<<(std::ostream &os, const Sphere &sphere);
};

void Sphere::expandToInclude(const Sphere &other) {
  float dist = Point::distance(center, other.center);

  if (dist + other.radius <= radius) {
    return;
  }

  if (dist + radius <= other.radius) {
    center = other.center;
    radius = other.radius;
    return;
  }

  float t = (dist + other.radius - radius) / (2.0f * dist);
  Point direction = other.center - center;

  radius = (dist + radius + other.radius) / 2.0f;
  center = center + (direction * t);
}

void Sphere::expandToInclude(const Point &p) {
  float dist = Point::distance(center, p);

  if (dist <= radius) {
    return;
  }

  float t = (dist - radius) / (2.0f * dist);

  radius = (radius + dist) / 2.0f;
  center = center + ((p - center) * t);
}

Sphere &Sphere::operator=(const Sphere &other) {
  if (this != &other) {
    center = other.center;
    radius = other.radius;
  }
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Sphere &sphere) {
  os << "(c = " << sphere.center << ", r = " << sphere.radius << ")\n";
  return os;
}

#endif // SPHERE_H
