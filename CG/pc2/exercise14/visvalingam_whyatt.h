#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

template <typename T>
struct Vertex {
  int index;
  double area;
  bool removed;
  Vertex<T> *prev;
  Vertex<T> *next;

  Vertex(int idx = -1, T a = T(0)) : index(idx), area(a), removed(false), prev(nullptr), next(nullptr) {}

  bool operator>(const Vertex &other) const {
    return area > other.area;
  }
};

template <typename T>
double calculate_triangle_area(const vector<T> &p1, const vector<T> &p2, const vector<T> &p3) {
  double area = abs(p1[0] * (p2[1] - p3[1]) + p2[0] * (p3[1] - p1[1]) + p3[0] * (p1[1] - p2[1])) / 2.0;
  return area;
}

template <typename T>
Vertex<T> *find_next(Vertex<T> *current) {
  return current ? current->next : nullptr;
}

template <typename T>
Vertex<T> *find_prev(Vertex<T> *current) {
  return current ? current->prev : nullptr;
}

template <typename T>
double calculate_area(const vector<vector<T>> &points, Vertex<T> *vertex) {
  Vertex<T> *prev = find_prev(vertex);
  Vertex<T> *next = find_next(vertex);

  if (!prev || !next) {
    return numeric_limits<T>::max();
  }

  return calculate_triangle_area(points[prev->index], points[vertex->index], points[next->index]);
}

template <typename T>
void remove(Vertex<T> *vertex) {
  if (vertex->prev) {
    vertex->prev->next = vertex->next;
  }
  if (vertex->next) {
    vertex->next->prev = vertex->prev;
  }
  vertex->removed = true;
}

template <typename T>
void update_areas(const vector<vector<T>> &points, vector<Vertex<T>> &vertices, priority_queue<Vertex<T>, vector<Vertex<T>>, greater<Vertex<T>>> &minHeap, Vertex<T> *removedVertex) {

  Vertex<T> *prev = removedVertex->prev;
  Vertex<T> *next = removedVertex->next;

  if (prev && prev->prev) {
    prev->area = calculate_area(points, prev);
    minHeap.push(*prev);
  }

  if (next && next->next) {
    next->area = calculate_area(points, next);
    minHeap.push(*next);
  }
}

template <typename T>
vector<vector<T>> triangles_based_simplification(vector<vector<T>> const &vertices, int N) {
  if (vertices.size() < 3 || N >= vertices.size()) {
    return vertices;
  }

  if (N < 3) {
    return {vertices.front(), vertices.back()};
  }

  vector<Vertex<T>> vertexData(vertices.size());
  priority_queue<Vertex<T>, vector<Vertex<T>>, greater<Vertex<T>>> minHeap;

  for (int i = 0; i < vertices.size(); i++) {
    vertexData[i].index = i;

    if (i > 0) {
      vertexData[i].prev = &vertexData[i - 1];
      vertexData[i - 1].next = &vertexData[i];
    }
  }

  for (int i = 0; i < vertices.size(); i++) {
    if (i == 0 || i == vertices.size() - 1) {
      vertexData[i].area = numeric_limits<T>::max();
    } else {
      vertexData[i].area = calculate_area(vertices, &vertexData[i]);
      minHeap.push(vertexData[i]);
    }
  }

  int to_remove = vertices.size() - N;

  for (int i = 0; i < to_remove; i++) {
    Vertex<T> minVertex;
    bool found = false;

    while (!minHeap.empty() && !found) {
      minVertex = minHeap.top();
      minHeap.pop();

      Vertex<T> *currentVertex = &vertexData[minVertex.index];

      if (!currentVertex->removed && currentVertex->prev && currentVertex->next) {
        found = true;
      }
    }

    if (!found)
      break;

    Vertex<T> *vertexToRemove = &vertexData[minVertex.index];
    update_areas(vertices, vertexData, minHeap, vertexToRemove);
    remove(vertexToRemove);
  }

  vector<vector<T>> result;

  Vertex<T> *current = &vertexData[0];
  while (current) {
    if (!current->removed) {
      result.push_back(vertices[current->index]);
    }
    current = current->next;
  }

  return result;
}

template <typename T>
void printVertices(const vector<vector<T>> &vertices, const string &label = "") {
  if (!label.empty()) {
    cout << label << ":\n";
  }
  for (size_t i = 0; i < vertices.size(); i++) {
    cout << "(";
    for (size_t j = 0; j < vertices[i].size(); j++) {
      cout << vertices[i][j];
      if (j < vertices[i].size() - 1)
        cout << ", ";
    }
    cout << ")";
    if (i < vertices.size() - 1)
      cout << " -> ";
  }
  cout << "\nTotal de vertices: " << vertices.size() << "\n\n";
}

template <typename T>
void printLinkedList(Vertex<T> *head, vector<vector<T>> vertices) {
  Vertex<T> *current = head;
  while (current) {
    cout << "(" << vertices[current->index][0] << ", " << vertices[current->index][1] << ") -> ";
    current = current->next;
  }
  cout << "\n";
}
