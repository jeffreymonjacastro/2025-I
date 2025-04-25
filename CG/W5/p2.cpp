#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct Point {
  float x, y;
  bool begin;

  Point() : x(0), y(0), begin(false) {}
  Point(float x, float y) : x(x), y(y), begin(false) {}
  Point(float x, float y, bool begin) : x(x), y(y), begin(begin) {}

  bool operator<(const Point &other) const {
    if (x != other.x) {
      return x < other.x;
    } else if (x == other.x) {
      return y < other.y;
    } else {
      return false;
    }
  }

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }

  friend ostream &operator<<(ostream &os, const Point &p) {
    os << "(" << p.x << ", " << p.y << ", " << (p.begin ? "B" : "E") << ")";
    return os;
  }
};

struct Segment {
  Point left, right;

  Segment(Point left, Point right) {
    if (left.x > right.x || (left.x == right.x && left.y > right.y)) {
      this->left = right;
      this->right = left;
    } else {
      this->left = left;
      this->right = right;
    }
  }

  friend ostream &operator<<(ostream &os, const Segment &s) {
    os << "Segment: " << s.left << " to " << s.right;
    return os;
  }
};

struct SegmentComparator {
  bool operator()(const Segment &a, const Segment &b) const {
    if (a.left.y != b.left.y) {
      return a.left.y < b.left.y;
    }
    if (a.right.y != b.right.y) {
      return a.right.y < b.right.y;
    }
    if (a.left.x != b.left.x) {
      return a.left.x < b.left.x;
    }
    return a.right.x < b.right.x;
  }
};

float direction(Point &p0, Point &p1, Point &p2) {
  return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool onSegment(Point &p0, Point &p1, Point &p2) {
  if (min(p0.x, p1.x) <= p2.x && p2.x <= max(p0.x, p1.x) &&
      min(p0.y, p1.y) <= p2.y && p2.y <= max(p0.y, p1.y)) {
    return true;
  }
  return false;
}

bool segmentsIntersect(Segment s1, Segment s2) {
  Point p1 = s1.left;
  Point p2 = s1.right;
  Point p3 = s2.left;
  Point p4 = s2.right;

  float d1 = direction(p3, p4, p1);
  float d2 = direction(p3, p4, p2);
  float d3 = direction(p1, p2, p3);
  float d4 = direction(p1, p2, p4);

  if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
      ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    return true;
  }

  if (d1 == 0 && onSegment(p3, p4, p1)) {
    return true;
  }

  if (d2 == 0 && onSegment(p3, p4, p2)) {
    return true;
  }

  if (d3 == 0 && onSegment(p1, p2, p3)) {
    return true;
  }

  if (d4 == 0 && onSegment(p1, p2, p4)) {
    return true;
  }

  return false;
}

bool anySegmentIntersect(vector<Segment> S) {
  vector<pair<Point, Segment>> events;

  for (Segment &s : S) {
    events.push_back(make_pair(s.left, s));
    events.push_back(make_pair(s.right, s));
  }

  sort(events.begin(), events.end(),
       [](const pair<Point, Segment> &a, const pair<Point, Segment> &b) {
         return a.first < b.first;
       });

  set<Segment, SegmentComparator> active;

  for (auto &event : events) {
    Point p = event.first;
    Segment s = event.second;

    if (p.begin) {
      active.insert(s);
      auto it = active.find(s);
      auto next = ++it;
      auto prev = --it;

      if (next != active.end() && segmentsIntersect(s, *next)) {
        cout << "Intersection found between " << s << " and " << *next << endl;
        return true;
      }

      if (prev != active.end() && segmentsIntersect(s, *prev)) {
        cout << "Intersection found between " << s << " and " << *prev << endl;
        return true;
      }
    } else {
      auto it = active.find(s);
      auto next = ++it;
      auto prev = --it;

      if (next != active.end() && prev != active.end() &&
          segmentsIntersect(*prev, *next)) {
        cout << "Intersection found between " << *prev << " and " << *next
             << endl;
        return true;
      }

      active.erase(it);
    }
  }

  return false;
}

int main() {
  Point p1(3, 2, true);
  Point p2(4, 1, false);
  Point p3(1, 1, true);
  Point p4(5, 3, false);

  Segment s1(p1, p2);
  Segment s2(p3, p4);

  vector<Segment> segments = {s1, s2};

  bool intersectionExists = anySegmentIntersect(segments);
  if (intersectionExists) {
    cout << "Segments intersect." << endl;
  } else {
    cout << "No intersections found." << endl;
  }

  return 0;
}