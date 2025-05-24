#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

template <typename T>
struct VertexData {
  int index;
  T area;
  bool removed;

  VertexData(int idx = -1, T a = T(0)) : index(idx), area(a), removed(false) {}

  // Para el priority_queue (min-heap)
  bool operator>(const VertexData &other) const {
    return area > other.area;
  }
};

template <typename T>
T calculateTriangleArea(const vector<T> &p1, const vector<T> &p2, const vector<T> &p3) {
  // Área = |x1(y2-y3) + x2(y3-y1) + x3(y1-y2)| / 2
  T area = abs(p1[0] * (p2[1] - p3[1]) + p2[0] * (p3[1] - p1[1]) + p3[0] * (p1[1] - p2[1])) / T(2);
  return area;
}

template <typename T>
int findNextValidVertex(const vector<VertexData<T>> &vertices, int current) {
  int next = current + 1;
  while (next < vertices.size() && vertices[next].removed) {
    next++;
  }
  return (next < vertices.size()) ? next : -1;
}

template <typename T>
int findPrevValidVertex(const vector<VertexData<T>> &vertices, int current) {
  int prev = current - 1;
  while (prev >= 0 && vertices[prev].removed) {
    prev--;
  }
  return (prev >= 0) ? prev : -1;
}

template <typename T>
T calculateEffectiveArea(const vector<vector<T>> &points, const vector<VertexData<T>> &vertices, int vertexIndex) {
  int prev = findPrevValidVertex(vertices, vertexIndex);
  int next = findNextValidVertex(vertices, vertexIndex);

  if (prev == -1 || next == -1) {
    return numeric_limits<T>::max(); // No se puede eliminar
  }

  return calculateTriangleArea(points[prev], points[vertexIndex], points[next]);
}

template <typename T>
void updateAffectedAreas(const vector<vector<T>> &points, vector<VertexData<T>> &vertices, priority_queue<VertexData<T>, vector<VertexData<T>>, greater<VertexData<T>>> &minHeap, int removedIndex) {
  int prev = findPrevValidVertex(vertices, removedIndex);
  int next = findNextValidVertex(vertices, removedIndex);

  // Actualizar área del vértice anterior
  if (prev != -1 && prev != 0) {
    vertices[prev].area = calculateEffectiveArea(points, vertices, prev);
    minHeap.push(vertices[prev]);
  }

  // Actualizar área del vértice siguiente
  if (next != -1 && next != points.size() - 1) {
    vertices[next].area = calculateEffectiveArea(points, vertices, next);
    minHeap.push(vertices[next]);
  }
}

template <typename T>
vector<vector<T>> triangles_based_simplification(vector<vector<T>> const &vertices, int N) {
  if (vertices.size() < 3 || N >= vertices.size()) {
    return vertices;
  }

  if (N < 2) {
    return {vertices.front(), vertices.back()};
  }

  vector<VertexData<T>> vertexData(vertices.size());
  priority_queue<VertexData<T>, vector<VertexData<T>>, greater<VertexData<T>>> minHeap;

  // Configurar datos iniciales de vértices
  for (int i = 0; i < vertices.size(); i++) {
    vertexData[i].index = i;

    if (i == 0 || i == vertices.size() - 1) {
      // Los puntos inicial y final no se pueden eliminar
      vertexData[i].area = numeric_limits<T>::max();
    } else {
      vertexData[i].area = calculateEffectiveArea(vertices, vertexData, i);
      minHeap.push(vertexData[i]);
    }
  }

  // Calcular cuántos puntos eliminar
  int pointsToRemove = vertices.size() - N;

  // Proceso de eliminación iterativa
  for (int removed = 0; removed < pointsToRemove; removed++) {
    // Encontrar el vértice con menor área válido
    VertexData<T> minVertex;
    bool found = false;

    while (!minHeap.empty() && !found) {
      minVertex = minHeap.top();
      minHeap.pop();

      // Verificar si este vértice aún es válido (no eliminado y área actual)
      if (!vertexData[minVertex.index].removed &&
          abs(vertexData[minVertex.index].area - minVertex.area) < numeric_limits<T>::epsilon() * 10) {
        found = true;
      }
    }

    if (!found)
      break; // No hay más vértices para eliminar

    // Marcar como eliminado
    vertexData[minVertex.index].removed = true;

    // Actualizar áreas de vértices afectados
    updateAffectedAreas(vertices, vertexData, minHeap, minVertex.index);
  }

  // Construir la línea simplificada
  vector<vector<T>> simplifiedVertices;
  simplifiedVertices.reserve(N);

  for (int i = 0; i < vertices.size(); i++) {
    if (!vertexData[i].removed) {
      simplifiedVertices.push_back(vertices[i]);
    }
  }

  return simplifiedVertices;
}

// Función de utilidad para imprimir vértices
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
  cout << "\nTotal de vértices: " << vertices.size() << "\n\n";
}
