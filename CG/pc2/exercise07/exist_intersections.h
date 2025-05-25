#ifndef EXIST_INTERSECTIONS_H
#define EXIST_INTERSECTIONS_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

#define ll long long int
#define ld long double

template <typename T = int>
struct Point {
  T x, y;
  Point(T x, T y) : x(x), y(y) {}
};

template <typename T = ll>
struct Segment {
  Point<T> left, right;
  Segment(Point<T> left, Point<T> right) : left(left), right(right) {
    if (left.x > right.x) {
      this->left = right;
      this->right = left;
    } else if (left.x == right.x && left.y > right.y) {
      this->left = right;
      this->right = left;
    } else {
      this->left = left;
      this->right = right;
    }
  }
};

ll cross_product(Point<int> p1, Point<int> p2, Point<int> p3) {
  ll x1 = (ll)p2.x - (ll)p1.x;
  ll y1 = (ll)p2.y - (ll)p1.y;
  ll x2 = (ll)p3.x - (ll)p1.x;
  ll y2 = (ll)p3.y - (ll)p1.y;
  return x1 * y2 - x2 * y1;
}

ld cross_product(Point<double> p1, Point<double> p2, Point<double> p3) {
  ld x1 = (ld)p2.x - (ld)p1.x;
  ld y1 = (ld)p2.y - (ld)p1.y;
  ld x2 = (ld)p3.x - (ld)p1.x;
  ld y2 = (ld)p3.y - (ld)p1.y;
  return x1 * y2 - x2 * y1;
}

template <typename T>
int sgn(T num) {
  return (num > 0) - (num < 0);
}

template <typename T>
bool intersection(T xa, T ya, T xb, T yb, T xc, T yc, T xd, T yd) {
  Point<T> p1(xa, ya), p2(xb, yb), p3(xc, yc), p4(xd, yd);
  Segment<T> s1(p1, p2), s2(p3, p4);

  auto P1P2_cross_P1P3 = cross_product(p1, p2, p3);
  int P1P2_cross_P1P3_sign = sgn(P1P2_cross_P1P3);
  auto P1P2_cross_P1P4 = cross_product(p1, p2, p4);
  int P1P2_cross_P1P4_sign = sgn(P1P2_cross_P1P4);

  if (P1P2_cross_P1P3 == 0 && P1P2_cross_P1P4 == 0) {
    T min_x1 = min(s1.left.x, s1.right.x);
    T max_x1 = max(s1.left.x, s1.right.x);
    T min_y1 = min(s1.left.y, s1.right.y);
    T max_y1 = max(s1.left.y, s1.right.y);
    T min_x2 = min(s2.left.x, s2.right.x);
    T max_x2 = max(s2.left.x, s2.right.x);
    T min_y2 = min(s2.left.y, s2.right.y);
    T max_y2 = max(s2.left.y, s2.right.y);
    if (min_x1 <= max_x2 && max_x1 >= min_x2 && min_y1 <= max_y2 && max_y1 >= min_y2) {
      return true;
    }
    return false;
  }

  if (P1P2_cross_P1P3_sign == P1P2_cross_P1P4_sign) {
    return false;
  }

  auto P3P4_cross_P3P1 = cross_product(p3, p4, p1);
  int P3P4_cross_P3P1_sign = sgn(P3P4_cross_P3P1);
  auto P3P4_cross_P3P2 = cross_product(p3, p4, p2);
  int P3P4_cross_P3P2_sign = sgn(P3P4_cross_P3P2);
  if (P3P4_cross_P3P1_sign == P3P4_cross_P3P2_sign) {
    return false;
  }
  return true;
}

template <typename T>
struct EventPoint {
  T x_coordinate;
  int segment_index;
  bool is_start_point;

  EventPoint(T x, int idx, bool start)
      : x_coordinate(x), segment_index(idx), is_start_point(start) {}

  bool operator<(const EventPoint &other) const {
    if (x_coordinate != other.x_coordinate) {
      return x_coordinate < other.x_coordinate;
    }
    return !is_start_point && other.is_start_point;
  }
};

template <typename T>
bool checkIntersectionsBruteForce(const vector<Segment<T>> &segments) {
  int n = segments.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (intersection(segments[i].left.x, segments[i].left.y,
                       segments[i].right.x, segments[i].right.y,
                       segments[j].left.x, segments[j].left.y,
                       segments[j].right.x, segments[j].right.y)) {
        return true;
      }
    }
  }
  return false;
}

template <typename T>
const vector<Segment<T>> *global_segments = nullptr;

template <typename T>
T global_current_x = T();

template <typename T>
auto segmentComparator = [](int idx1, int idx2) -> bool {
  const Segment<T> &seg1 = (*global_segments<T>)[idx1];
  const Segment<T> &seg2 = (*global_segments<T>)[idx2];

  ld y1, y2;
  if (seg1.left.x == seg1.right.x) {
    y1 = seg1.left.y;
  } else {
    ld slope1 = (ld)(seg1.right.y - seg1.left.y) / (seg1.right.x - seg1.left.x);
    y1 = seg1.left.y + slope1 * (global_current_x<T> - seg1.left.x);
  }

  if (seg2.left.x == seg2.right.x) {
    y2 = seg2.left.y;
  } else {
    ld slope2 = (ld)(seg2.right.y - seg2.left.y) / (seg2.right.x - seg2.left.x);
    y2 = seg2.left.y + slope2 * (global_current_x<T> - seg2.left.x);
  }

  if (abs(y1 - y2) > 1e-9) {
    return y1 < y2;
  }
  return idx1 < idx2;
};

template <typename T>
bool exist_intersections(vector<vector<T>> const &segments) {
  vector<Segment<T>> segment_list;
  for (const auto &coords : segments) {
    if (coords.size() >= 4) {
      Point<T> p1(coords[0], coords[1]);
      Point<T> p2(coords[2], coords[3]);
      segment_list.push_back(Segment<T>(p1, p2));
    }
  }

  if (segment_list.size() < 2)
    return false;

  global_segments<T> = &segment_list;

  vector<EventPoint<T>> event_queue;
  for (int i = 0; i < segment_list.size(); i++) {
    event_queue.push_back(EventPoint<T>(segment_list[i].left.x, i, true));
    event_queue.push_back(EventPoint<T>(segment_list[i].right.x, i, false));
  }

  sort(event_queue.begin(), event_queue.end());

  set<int, decltype(segmentComparator<T>)> active_segments(segmentComparator<T>);

  for (const auto &event : event_queue) {
    global_current_x<T> = event.x_coordinate;
    int seg_idx = event.segment_index;

    if (event.is_start_point) {
      auto insertion_result = active_segments.insert(seg_idx);
      auto current_iter = insertion_result.first;

      if (current_iter != active_segments.begin()) {
        auto prev_iter = prev(current_iter);
        if (intersection(segment_list[*prev_iter].left.x, segment_list[*prev_iter].left.y,
                         segment_list[*prev_iter].right.x, segment_list[*prev_iter].right.y,
                         segment_list[seg_idx].left.x, segment_list[seg_idx].left.y,
                         segment_list[seg_idx].right.x, segment_list[seg_idx].right.y)) {
          return true;
        }
      }

      auto next_iter = next(current_iter);
      if (next_iter != active_segments.end()) {
        if (intersection(segment_list[seg_idx].left.x, segment_list[seg_idx].left.y,
                         segment_list[seg_idx].right.x, segment_list[seg_idx].right.y,
                         segment_list[*next_iter].left.x, segment_list[*next_iter].left.y,
                         segment_list[*next_iter].right.x, segment_list[*next_iter].right.y)) {
          return true;
        }
      }
    } else {
      auto current_iter = active_segments.find(seg_idx);
      if (current_iter != active_segments.end()) {
        auto prev_iter = (current_iter != active_segments.begin()) ? prev(current_iter) : active_segments.end();
        auto next_iter = next(current_iter);

        if (prev_iter != active_segments.end() &&
            next_iter != active_segments.end()) {
          if (intersection(segment_list[*prev_iter].left.x, segment_list[*prev_iter].left.y,
                           segment_list[*prev_iter].right.x, segment_list[*prev_iter].right.y,
                           segment_list[*next_iter].left.x, segment_list[*next_iter].left.y,
                           segment_list[*next_iter].right.x, segment_list[*next_iter].right.y)) {
            return true;
          }
        }

        active_segments.erase(current_iter);
      }
    }
  }

  return false;
}

struct IntersectionDetector {
  template <typename T>
  static bool detectWithBruteForce(const vector<vector<T>> &segment_coords) {
    vector<Segment<T>> segments;
    for (const auto &coords : segment_coords) {
      if (coords.size() >= 4) {
        Point<T> p1(coords[0], coords[1]);
        Point<T> p2(coords[2], coords[3]);
        segments.push_back(Segment<T>(p1, p2));
      }
    }
    return checkIntersectionsBruteForce(segments);
  }

  template <typename T>
  static bool detectWithSweepLine(const vector<vector<T>> &segment_coords) {
    return exist_intersections(segment_coords);
  }

  template <typename T>
  static bool detect(const vector<vector<T>> &segment_coords, const string &method = "sweep") {
    if (method == "brute" || method == "bruteforce") {
      return detectWithBruteForce(segment_coords);
    } else {
      return detectWithSweepLine(segment_coords);
    }
  }
};

#endif