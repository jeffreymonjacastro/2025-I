#include <iostream>
#include <cmath>
#include <ctime>
#include <cassert>
#include "Rtree.h"

bool approxEqual(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}
bool mbbEqual(const MBB &a, const MBB &b, float epsilon = 1e-5f) {
    return approxEqual(a.lower.x, b.lower.x, epsilon) &&
           approxEqual(a.lower.y, b.lower.y, epsilon) &&
           approxEqual(a.upper.x, b.upper.x, epsilon) &&
           approxEqual(a.upper.y, b.upper.y, epsilon);
}

// Test: verifica las propiedades de R-tree
bool checkTree(RNode* node, int maxEntries, bool isRoot, int currentDepth, int &leafDepth) {
    bool ok = true;
    int minCapacity = (maxEntries + 1) / 2;
    
    if (node->isLeaf) {
        // Verificar que en un nodo hoja NO tiene children
        if (!node->children.empty()) {
            std::cerr << "Error: Nodo hoja no debe tener hijos. Tiene " << node->children.size() << " hijos." 
                      << std::endl;
            ok = false;
        }

        size_t pointCount = node->points.size();

        // Verificar capacidad en nodo hoja (si no es root)
        if (!isRoot) {
            if (pointCount < (size_t)minCapacity) {
                std::cerr << "Error: Nodo hoja. Cantidad actual de puntos: " << pointCount 
                          << ". Se esperaba al menos " << minCapacity << " puntos." << std::endl;
                ok = false;
            }
        }
        if (pointCount > (size_t)maxEntries) {
            std::cerr << "Error: Nodo hoja. Cantidad actual de puntos: " << pointCount 
                      << ". Se esperaba como maximo " << maxEntries << " puntos." << std::endl;
            ok = false;
        }

        // Comprobar que el MBB es mínimo: debe coincidir con el calculado a partir de los puntos.
        MBB computed;
        if (!node->points.empty()) {
            computed.lower = node->points[0];
            computed.upper = node->points[0];
            for (size_t i = 1; i < node->points.size(); i++) {
                computed.lower.x = std::min(computed.lower.x, node->points[i].x);
                computed.lower.y = std::min(computed.lower.y, node->points[i].y);
                computed.upper.x = std::max(computed.upper.x, node->points[i].x);
                computed.upper.y = std::max(computed.upper.y, node->points[i].y);
            }
        }
        if (!mbbEqual(node->mbr, computed)) {
            std::cerr << "Error: MBB minimo inconsistente en nodo hoja. " 
                      << "Se esperaba " << "{" << computed.lower.x << "," << computed.lower.y 
                      << "}-{" << computed.upper.x << "," << computed.upper.y << "}, "
                      << "pero se tiene " << "{" << node->mbr.lower.x << "," << node->mbr.lower.y 
                      << "}-{" << node->mbr.upper.x << "," << node->mbr.upper.y << "}" << std::endl;
            ok = false;
        }

        // Verificar balance: la profundidad de todas las hojas debe ser la misma.
        if (leafDepth == -1)
            leafDepth = currentDepth;
        else if (leafDepth != currentDepth) {
            std::cerr << "Error: Arbol desequilibrado. Una hoja tiene profundidad " << currentDepth 
                      << " y se esperaba que todas las hojas estuvieran en la misma profundidad (" 
                      << leafDepth << ")." << std::endl;
            ok = false;
        }

    } else {
        // Verificar que en un nodo interno NO no tenga puntos
        if (!node->points.empty()) {
            std::cerr << "Error: Nodo interno no debe tener puntos. Tiene " << node->points.size() << " puntos." 
                      << std::endl;
            ok = false;
        }

        size_t childCount = node->children.size();

        // Nodo interno
        if (!isRoot) {
            if (childCount < (size_t)minCapacity) {
                std::cerr << "Error: Nodo interno. Cantidad actual de hijos: " << childCount 
                          << ". Se esperaba al menos " << minCapacity << " hijos." << std::endl;
                ok = false;
            }
        }
        if (childCount > (size_t)maxEntries) {
            std::cerr << "Error: Nodo interno. Cantidad actual de hijos: " << childCount 
                      << ". Se esperaba como máximo " << maxEntries << " hijos." << std::endl;
            ok = false;
        }

        // Verificar que el MBB es mínimo
        MBB computed;
        if (!node->children.empty()) {
            computed.lower = node->children[0]->mbr.lower;
            computed.upper = node->children[0]->mbr.upper;
            for (size_t i = 1; i < node->children.size(); i++) {
                computed.lower.x = std::min(computed.lower.x, node->children[i]->mbr.lower.x);
                computed.lower.y = std::min(computed.lower.y, node->children[i]->mbr.lower.y);
                computed.upper.x = std::max(computed.upper.x, node->children[i]->mbr.upper.x);
                computed.upper.y = std::max(computed.upper.y, node->children[i]->mbr.upper.y);
            }
        }
        if (!mbbEqual(node->mbr, computed)) {
            std::cerr << "Error: MBB minimo inconsistente en nodo interno. " 
                      << "Se esperaba " << "{" << computed.lower.x << "," << computed.lower.y 
                      << "}-{" << computed.upper.x << "," << computed.upper.y << "}, "
                      << "pero se tiene " << "{" << node->mbr.lower.x << "," << node->mbr.lower.y 
                      << "}-{" << node->mbr.upper.x << "," << node->mbr.upper.y << "}" << std::endl;
            ok = false;
        }

        // Cada hijo debe tener su MBB contenido en el MBB del padre
        for (RNode* child : node->children) {
            if (!(node->mbr.lower.x <= child->mbr.lower.x &&
                  node->mbr.lower.y <= child->mbr.lower.y &&
                  node->mbr.upper.x >= child->mbr.upper.x &&
                  node->mbr.upper.y >= child->mbr.upper.y)) {
                std::cerr << "Error: El MBB de un hijo no está contenido en el nodo interno. "
                          << "MBB del padre: {" << node->mbr.lower.x << "," << node->mbr.lower.y 
                          << "}-{" << node->mbr.upper.x << "," << node->mbr.upper.y << "}, "
                          << "MBB del hijo: {" << child->mbr.lower.x << "," << child->mbr.lower.y 
                          << "}-{" << child->mbr.upper.x << "," << child->mbr.upper.y << "}" << std::endl;
                ok = false;
            }

            // Check childrens
            if (!checkTree(child, maxEntries, false, currentDepth + 1, leafDepth))
                ok = false;
        }
    }
    return ok;
}
void testTreeStructure(RTree &tree) {
    std::cout << "Verificando invariantes avanzadas del árbol..." << std::endl;
    int leafDepth = -1;
    if (checkTree(tree.root, tree.maxEntries, true, 0, leafDepth))
        std::cout << "El árbol cumple con todas las invariantes avanzadas." << std::endl;
    else
        std::cerr << "ERROR: Fallaron las invariantes avanzadas del árbol." << std::endl;
}


// Test: Probar range query
void testSearch(RTree &tree, const std::vector<Point> &allPoints) {
    std::cout << "Ejecutando tests de búsqueda..." << std::endl;

    const int numQueries = 50;
    for (int i = 0; i < numQueries; i++) {
        // Random region
        float x1 = static_cast<float>(std::rand() % 100);
        float y1 = static_cast<float>(std::rand() % 100);
        float x2 = static_cast<float>(std::rand() % 100);
        float y2 = static_cast<float>(std::rand() % 100);
        MBB query(Point(std::min(x1, x2), std::min(y1, y2)),
                  Point(std::max(x1, x2), std::max(y1, y2)));

        std::vector<Point> result = tree.search(query);

        // Fuerza bruta
        std::vector<Point> expected;
        for (const auto &pt : allPoints) {
            if (pt.x >= query.lower.x && pt.x <= query.upper.x &&
                pt.y >= query.lower.y && pt.y <= query.upper.y)
                expected.push_back(pt);
        }

        // Ordenamos y comparamos
        auto cmp = [](const Point &a, const Point &b) { return a.x < b.x || (approxEqual(a.x, b.x) && a.y < b.y); };
        std::sort(result.begin(), result.end(), cmp);
        std::sort(expected.begin(), expected.end(), cmp);
        if (result.size() != expected.size()) {
            std::cerr << "Error en búsqueda: para query (" << query.lower.x << "," << query.lower.y
                      << ")-(" << query.upper.x << "," << query.upper.y << ") se esperaban "
                      << expected.size() << " resultados, pero se obtuvieron " << result.size() << ".\n";
            continue;
        }
        for (size_t j = 0; j < result.size(); j++) {
            if (!approxEqual(result[j].x, expected[j].x) || !approxEqual(result[j].y, expected[j].y)) {
                std::cerr << "Error en búsqueda: los puntos retornados no coinciden con los esperados.\n";
                break;
            }
        }
    }
    std::cout << "Pruebas de búsqueda completadas." << std::endl;
}






// Test: Verificar kNN
void testKNN(RTree &tree, const std::vector<Point> &allPoints) {
    std::cout << "Ejecutando tests de kNN..." << std::endl;
    const int numTests = 10;
    for (int t = 0; t < numTests; t++) {
        // Random query
        float qx = static_cast<float>(std::rand() % 100);
        float qy = static_cast<float>(std::rand() % 100);
        Point query(qx, qy);

        // k entre 1 y 20
        int k = std::rand() % 20 + 1;
        std::cout << "Test " << (t + 1)
                  << ": Query (" << qx << ", " << qy << "), k = " << k << std::endl;

        // Fuerza bruta
        std::vector<Point> naive = allPoints;
        std::sort(naive.begin(), naive.end(), [&query](const Point &a, const Point &b) {
            return a.distanceTo(query) < b.distanceTo(query);
        });
        if (naive.size() > (size_t)k)
            naive.resize(k);

        // Tu solucion
        std::vector<Point> knn = tree.kNN(query, k);

        if (knn.size() != naive.size()) {
            std::cerr << "Error en kNN: se esperaban " << naive.size()
                      << " puntos, pero se obtuvieron " << knn.size() << ".\n";
            continue;
        }
        for (size_t i = 0; i < knn.size(); i++) {
            float d1 = knn[i].distanceTo(query);
            float d2 = naive[i].distanceTo(query);
            if (!approxEqual(d1, d2)) {
                std::cerr << "Error en kNN: diferencia en la distancia del " << i
                          << "-esimo vecino (kNN=" << d1 << ", ingenuo=" << d2 << ").\n";
            }
        }
    }
    std::cout << "Pruebas de kNN completadas." << std::endl;
}










int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<Point> allPoints;
    const int numPoints = 2000;
    for (int i = 0; i < numPoints; i++) {
        float x = static_cast<float>(std::rand() % 100);
        float y = static_cast<float>(std::rand() % 100);
        allPoints.push_back(Point(x, y));
    }

    int maxEntries = 11;
    RTree tree(maxEntries);
    for (const auto &pt : allPoints) {
        tree.insert(pt);
    }

      testTreeStructure(tree);
      testSearch(tree, allPoints);
      testKNN(tree, allPoints);

    std::cout << "\nTodos los tests avanzados completados." << std::endl;

    return 0;
}
