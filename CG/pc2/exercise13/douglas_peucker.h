#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

template <typename T>
double distance_point_to_line_2D(vector<T> const &p1, vector<T> const &p2, vector<T> const &p) {
  double dx, dy;

  if (p1[0] == p2[0] && p1[1] == p2[1]) {
    dx = (double)(p[0] - p1[0]);
    dy = (double)(p[1] - p1[1]);
    return sqrt(dx * dx + dy * dy);
  }

  dx = (double)(p2[0] - p1[0]);
  dy = (double)(p2[1] - p1[1]);

  double modulo = dx * dx + dy * dy;

  double px = (double)(p[0] - p1[0]);
  double py = (double)(p[1] - p1[1]);

  double cross = abs(px * dy - py * dx);
  return cross / sqrt(modulo);
}

struct Segment {
  int start;
  int end;

  Segment(int s, int e) : start(s), end(e) {}
};

template <typename T>
vector<vector<T>> douglas_peucker(vector<vector<T>> const &vertices, double epsilon) {
  if (vertices.size() < 3) {
    return vertices;
  }

  vector<bool> keep(vertices.size(), false);
  keep[0] = true;
  keep[vertices.size() - 1] = true;

  stack<Segment> segments;
  segments.push(Segment(0, vertices.size() - 1));

  while (!segments.empty()) {
    Segment curr = segments.top();
    segments.pop();

    double maxDistance = 0.0;
    int maxIndex = curr.start;

    for (int i = curr.start + 1; i < curr.end; ++i) {
      double distance = distance_point_to_line_2D(vertices[curr.start], vertices[curr.end], vertices[i]);
      if (distance > maxDistance) {
        maxDistance = distance;
        maxIndex = i;
      }
    }

    if (maxDistance > epsilon) {
      keep[maxIndex] = true;

      segments.push(Segment(curr.start, maxIndex));
      segments.push(Segment(maxIndex, curr.end));
    }
  }

  vector<vector<T>> res;
  for (size_t i = 0; i < keep.size(); ++i) {
    if (keep[i]) {
      res.push_back(vertices[i]);
    }
  }

  return res;
}