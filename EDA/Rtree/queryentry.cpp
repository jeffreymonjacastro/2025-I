#include <queue>
#include <vector>
#include <iostream>
#include <cmath>

// Definición de las estructuras
class Point {
public:
    float x, y;
    Point() : x(0.0f), y(0.0f) {}
    Point(float x, float y) : x(x), y(y) {}

    // Distancia euclidiana
    float distanceTo(const Point &other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

struct QueueEntry {
    float distance;
    bool isNode;
    void* node;
    Point pt;
};

struct QueueEntryComparator {
    bool operator()(const QueueEntry &a, const QueueEntry &b) const {
        return a.distance > b.distance;  // Menor distancia, mayor prioridad
    }
};

int main() {
    // Crear un priority_queue de QueueEntry, usando QueueEntryComparator
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryComparator> pq;

    // Aquí puedes agregar elementos a la priority_queue
    QueueEntry entry1 = {10.5f, true, nullptr, Point(1, 2)};
    QueueEntry entry2 = {5.5f, false, nullptr, Point(3, 4)};
    QueueEntry entry3 = {2.5f, true, nullptr, Point(5, 6)};

    pq.push(entry1);
    pq.push(entry2);
    pq.push(entry3);

    // Mostrar los elementos de la priority_queue (en orden de prioridad)
    while (!pq.empty()) {
        QueueEntry entry = pq.top();
        pq.pop();
        std::cout << "Distance: " << entry.distance << ", Is Node: " << entry.isNode << std::endl;
    }

    return 0;
}
