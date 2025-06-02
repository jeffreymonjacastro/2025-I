#ifndef MBB_H
#define MBB_H

#include "Point.h"

struct MBB {
    Point minCorner, maxCorner;

    MBB() 
      : minCorner(), maxCorner() {}
    explicit MBB(const Point& p) 
      : minCorner(p), maxCorner(p) {}
    MBB(const Point& min, const Point& max)
      : minCorner(min), maxCorner(max) {}
    MBB(const MBB& other)
        : minCorner(other.minCorner), maxCorner(other.maxCorner) {}

    void expandToInclude(const MBB& other);
    void expandToInclude(const Point& p);
    static float maxDist(const Point& p, const MBB& box);

};

#endif // MBB_H
