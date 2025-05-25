#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
bool exist_intersections(vector<vector<T>> const &segments) {
}

#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()

using namespace std;

using Ntype = int;
static Ntype curr_sweep;

template <typename T>
struct Point {
  T x, y;

  Point() {};

  Point(T _x, T _y) {
    x = _x;
    y = _y;
  }
};

template <typename T>
struct Segment {
  Point<T> p1, p2;

  Segment() {};

  Segment(const Point<T> &_p1, const Point<T> &_p2) {
    if (_p1.x < _p2.x) {
      p1 = _p1;
      p2 = _p2;
    } else {
      p1 = _p2;
      p2 = _p1;
    }
  }
};

template <typename T>
struct Event {
  Point<T> p;
  int id, tag;
  Event(const Point<T> &_p, int _idx, int _tag) {
    this->p = _p;
    id = _idx;
    tag = _tag;
  }
  bool operator<(const Event<T> &other) const {
    if (this->p.x == other.p.x) {
      return this->tag > other.tag;
    }
    return this->p.x < other.p.x;
  }
};

template <typename T>
float cross_product(T Ax, T Ay, T Bx, T By) {
  return Ax * By - Bx * Ay;
}

template <typename T>
float dot_product(T Ax, T Ay, T Bx, T By) {
  return Ax * Bx + Ay * By;
}

template <typename T>
int sign(T t) {
  if (t < 0) {
    return -1;
  }
  if (t == 0) {
    return 0;
  }

  return 1;
}

template <typename T>
bool intersects(Segment<T> s1, Segment<T> s2) {
  // P1P3 X P1P2
  // P1P4 X P1P2
  T x1 = s1.p1.x, y1 = s1.p1.y, x2 = s1.p2.x, y2 = s1.p2.y, x3 = s2.p1.x, y3 = s2.p1.y,
    x4 = s2.p2.x, y4 = s2.p2.y;
  T a = cross_product(x3 - x1, y3 - y1, x2 - x1, y2 - y1);
  T b = cross_product(x4 - x1, y4 - y1, x2 - x1, y2 - y1);
  T c = cross_product(x1 - x3, y1 - y3, x4 - x3, y4 - y3);
  T d = cross_product(x2 - x3, y2 - y3, x4 - x3, y4 - y3);
  if ((sign(a) == 0 && sign(b) == 0) || (sign(c) == 0 && sign(d) == 0)) {
    return sign(dot_product(x3 - x2, y3 - y2, x3 - x1, y3 - y1)) <= 0 || sign(dot_product(x4 - x2, y4 - y2, x4 - x1, y4 - y1)) <= 0 || sign(dot_product(x1 - x3, y1 - y3, x1 - x4, y1 - y4)) <= 0 ||
           sign(dot_product(x2 - x3, y2 - y3, x2 - x4, y2 - y4)) <= 0;
  }

  if (sign(a) == sign(b)) {
    return false;
  }

  if (sign(c) == sign(d)) {
    return false;
  }
  return true;
}

template <typename T>
bool exist_intersections_handle_overflow(vector<vector<T>> &segments) {
  int n = sz(segments);
  vector<Segment<T>> seg;
  for (auto &s : segments) {
    Point<T> p1(s[0], s[1]);
    Point<T> p2(s[2], s[3]);
    seg.push_back(Segment<T>(p1, p2));
  }

  // cout<<intersects(seg[1] , seg[0])<<endl;

  vector<Event<T>> events;
  for (int i = 0; i < n; i++) {
    auto e1 = Event(seg[i].p1, i, 1);
    auto e2 = Event(seg[i].p2, i, -1);
    events.push_back(e1);
    events.push_back(e2);
  }
  sort(all(events));

  auto cmp = [&](int i, int j) {
    Segment<T> A = seg[i];
    Segment<T> B = seg[j];

    T deltaXA = A.p2.x - A.p1.x;
    T deltaYA = A.p2.y - A.p1.y;
    T deltasweepA = curr_sweep - A.p1.x;

    T deltaXB = B.p2.x - B.p1.x;
    T deltaYB = B.p2.y - B.p1.y;
    T deltasweepB = curr_sweep - B.p1.x;

    T fA = A.p1.y * deltaXA + deltaYA * (deltasweepA);
    T fB = B.p1.y * deltaXB + deltaYB * (deltasweepB);

    if (fA * deltaXB != fB * deltaXA)
      return fA * deltaXB < fB * deltaXA;
    return i < j;
  };

  set<int, decltype(cmp)> tree(cmp);

  for (auto &e : events) {
    int i = e.id;
    int type = e.tag;
    curr_sweep = e.p.x;
    if (type == 1) {

      auto it = tree.insert(i).first;
      // cout<<"size : "<<sz(tree)<<endl;
      auto prec = it == tree.begin() ? tree.end() : prev(it);
      auto sus = next(it);

      if (prec != tree.end() && intersects(seg[i], seg[*prec])) {
        return true;
      }
      if (sus != tree.end() && intersects(seg[i], seg[*sus]))
        return true;
    } else {
      auto it = tree.find(i);
      auto prec = it == tree.begin() ? tree.end() : prev(it);
      auto sus = next(it);
      if (prec != tree.end() && sus != tree.end() &&
          intersects(seg[*prec], seg[*sus]))
        return true;
      tree.erase(it);
    }
  }

  return false;
}

template <typename T>
bool exist_intersections_bf_handle_overflow(vector<vector<T>> &segments) {
  int n = sz(segments);
  vector<Segment<T>> seg;
  for (auto &s : segments) {
    Point<T> p1(s[0], s[1]);
    Point<T> p2(s[2], s[3]);
    seg.push_back(Segment<T>(p1, p2));
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (intersects(seg[i], seg[j])) {
        return true;
      }
    }
  }
  return false;
}

template <typename T>
bool exist_intersections_bf(vector<vector<T>> &segments) {
  if constexpr (is_same<T, int>::value) {
    // cout<<"int"<<endl;
    vector<vector<long long int>> segs;
    for (int i = 0; i < sz(segments); i++) {
      vector<long long int> seg(4);
      for (int j = 0; j < 4; j++)
        seg[j] = segments[i][j];
      segs.push_back(seg);
    }
    return exist_intersections_bf_handle_overflow(segs);
  } else {
    // cout<<"double"<<endl;
    vector<vector<long double>> segs;
    for (int i = 0; i < sz(segments); i++) {
      vector<long double> seg(4);
      for (int j = 0; j < 4; j++)
        seg[j] = segments[i][j];
      segs.push_back(seg);
    }
    return exist_intersections_bf_handle_overflow(segs);
  }
}

// template of PC2
template <typename T>
bool exist_intersections(vector<vector<T>> const &segments) {
  if constexpr (is_same<T, int>::value) {
    // cout<<"int"<<endl;
    vector<vector<long long int>> segs;
    for (int i = 0; i < sz(segments); i++) {
      vector<long long int> seg(4);
      for (int j = 0; j < 4; j++)
        seg[j] = segments[i][j];
      segs.push_back(seg);
    }
    return exist_intersections_handle_overflow(segs);
  } else {
    // cout<<"double"<<endl;
    vector<vector<long double>> segs;
    for (int i = 0; i < sz(segments); i++) {
      vector<long double> seg(4);
      for (int j = 0; j < 4; j++)
        seg[j] = segments[i][j];
      segs.push_back(seg);
    }
    return exist_intersections_handle_overflow(segs);
  }
}

#define TC 11

int main() {
  vector<vector<int>> tc[TC] = {
      {{0, 0, 2, 2}, {0, 2, 2, 0}},
      {{0, 0, 1, 1}, {2, 2, 3, 3}},
      {{0, 0, 4, 4}, {1, 1, 2, 2}},
      {{0, 0, 3, 1}, {1, 2, 4, 0}},
      {{0, 1, 2, 3}, {2, 3, 4, 5}},
      {{0, 0, 5, 1}, {1, 2, 6, 3}, {2, 4, 7, 5}},
      {{0, 0, 5, 5}, {5, 0, 6, 1}, {5, 5, 6, 4}},
      {{0, 0, 3, 1}, {3, 1, 5, 0}, {1, 2, 4, 2}},
      {{0, 0, 4, 2}, {2, 4, 6, 0}, {1, 3, 5, 1}},
      {{0, 2, 3, 0}, {2, 4, 5, 2}, {1, 3, 4, 1}, {3, 5, 6, 3}},
      {{3, 3, 6, 4}, {8, 8, 10, 5}, {2, 8, 5, 5}, {1, 5, 5, 8}}};

  for (int i = 0; i < TC; i++) {
    if (exist_intersections_bf(tc[i]) != exist_intersections(tc[i])) {
      cout << "Fail test " << (i + 1) << endl;
      cout << "Brute Force: " << exist_intersections_bf(tc[i]) << endl;
      cout << "Sweep Line : " << exist_intersections(tc[i]) << endl;
    }
    cout << "Test " << (i + 1) << " passed!" << endl;
    cout << "Brute Force: " << exist_intersections_bf(tc[i]) << endl;
    cout << "Sweep Line : " << exist_intersections(tc[i]) << endl;
    cout << "--------------------------------------" << endl;
  }

  return 0;
}