#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <array>
#include <functional>
#include <string>

#include "Point.h"
#include "MBB.h"
#include "Sphere.h"
#include "SRtree.h"

static constexpr float FLOAT_TOL = 1e-6f;

bool equalPoint(const Point& a, const Point& b) {
    for (std::size_t i = 0; i < DIM; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool pointInBox(const Point& p, const MBB& box) {
    for (std::size_t i = 0; i < DIM; ++i) {
        float coord = p[i];
        if (coord < box.minCorner[i] - FLOAT_TOL || coord > box.maxCorner[i] + FLOAT_TOL) {
            return false;
        }
    }
    return true;
}

bool pointInSphere(const Point& p, const Sphere& s) {
    float d = Point::distance(p, s.center);
    return d <= s.radius + FLOAT_TOL;
}

bool samePointSet(const std::vector<Point>& A, const std::vector<Point>& B) {
    if (A.size() != B.size()) return false;
    std::vector<bool> matched(B.size(), false);

    for (const Point& a : A) {
        bool found = false;
        for (std::size_t j = 0; j < B.size(); ++j) {
            if (!matched[j] && equalPoint(a, B[j])) {
                matched[j] = true;
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

bool sameDistanceList(const std::vector<float>& A, const std::vector<float>& B) {
    if (A.size() != B.size()) return false;
    for (std::size_t i = 0; i < A.size(); ++i) {
        if (std::fabs(A[i] - B[i]) > FLOAT_TOL) return false;
    }
    return true;
}

void collectPointsFromSubtree(const SRNode* node, std::vector<Point*>& outPoints) {
    if (!node) return;
    if (node->getIsLeaf()) {
        for (Point* ptr : node->getPoints()) {
            outPoints.push_back(ptr);
        }
    } else {
        for (SRNode* child : node->getChildren()) {
            collectPointsFromSubtree(child, outPoints);
        }
    }
}

// -------------------------------------------------------------
// TEST 1: Verificar bounding volumes (MBB y Sphere)
// -------------------------------------------------------------
bool testBoundingVolumes(const SRTree& tree) {
    const SRNode* root = tree.getRoot();
    if (!root) {
        std::cerr << "[ERROR] Test 1 (Bounding Volumes): El árbol está vacío.\n";
        return false;
    }

    bool allOK = true;

    std::function<void(const SRNode*, const std::string&)> recurse;
    recurse = [&](const SRNode* node, const std::string& path) {
        std::vector<Point*> ptsUnder;
        collectPointsFromSubtree(node, ptsUnder);

        // Verificar MBB
        const MBB& box = node->getBoundingBox();
        for (Point* pptr : ptsUnder) {
            if (!pointInBox(*pptr, box)) {
                allOK = false;
                break;
            }
        }

        // Verificar Sphere
        const Sphere& sph = node->getBoundingSphere();
        for (Point* pptr : ptsUnder) {
            if (!pointInSphere(*pptr, sph)) {
                allOK = false;
                break;
            }
        }

        // Nodo interno: revisar con cada hijo
        if (!node->getIsLeaf()) {
            for (SRNode* child : node->getChildren()) {
                recurse(child, "");
            }
        }
    };

    recurse(root, "root");

    if (allOK) {
        std::cout << "[OK] Test 1 (Bounding Volumes) pasó correctamente.\n";
    } else {
        std::cout << "[ERROR] Test 1 (Bounding Volumes) falló.\n";
    }
    return allOK;
}

// -------------------------------------------------------------
// TEST 2: Verificar search
// -------------------------------------------------------------
bool testSearch(const SRTree& tree, const std::vector<Point>& allPoints) {
    bool allOK = true;
    for (std::size_t i = 0; i < allPoints.size(); ++i) {
        if (!tree.search(allPoints[i])) {
            allOK = false;
            break;
        }
    }
    Point extra = Point::random(0.0f, 1.0f);
    if (tree.search(extra)) {
        allOK = false;
    }

    if (allOK) {
        std::cout << "[OK] Test 2 (Search) pasó correctamente.\n";
    } else {
        std::cout << "[ERROR] Test 2 (Search) falló :'c.\n";
    }
    return allOK;
}

// -------------------------------------------------------------
// TEST 3: rangeQuery(MBB)
// -------------------------------------------------------------
bool testRangeQueryBox(const SRTree& tree, const std::vector<Point>& allPoints) {
    bool allOK = true;
    int failures = 0;
    constexpr int NUM_TESTS = 5;

    std::mt19937 gen(54321); 
    std::uniform_int_distribution<std::size_t> distIdx(0, allPoints.size() - 1);

    for (int t = 0; t < NUM_TESTS; ++t) {
        const Point& a = allPoints[distIdx(gen)];
        const Point& b = allPoints[distIdx(gen)];

        std::array<float, DIM> cmin, cmax;
        for (std::size_t i = 0; i < DIM; ++i) {
            cmin[i] = std::min(a[i], b[i]);
            cmax[i] = std::max(a[i], b[i]);
        }
        MBB box(Point(cmin), Point(cmax));

        // SR-Tree
        std::vector<Point*> ptrs = tree.rangeQuery(box);
        std::vector<Point> treeRes;
        treeRes.reserve(ptrs.size());
        for (Point* pptr : ptrs) {
            treeRes.push_back(*pptr);
        }

        // Fuerza bruta
        std::vector<Point> brute;
        for (const Point& p : allPoints) {
            if (pointInBox(p, box)) {
                brute.push_back(p);
            }
        }

        // Comparar
        if (!samePointSet(treeRes, brute)) {
            failures++;
        }
    }

    if (failures == 0) {
        std::cout << "[OK] Test 3 (rangeQuery MBB) pasó todos los subtests (" 
                  << NUM_TESTS << "/" << NUM_TESTS << ").\n";
    } else {
        std::cout << "[ERROR] Test 3 (rangeQuery MBB): fallaron " 
                  << failures << " de " << NUM_TESTS << " subtests.\n";
        allOK = false;
    }
    return allOK;
}

// -------------------------------------------------------------
// TEST 4: rangeQuery(Sphere)
// -------------------------------------------------------------
bool testRangeQuerySphere(const SRTree& tree, const std::vector<Point>& allPoints) {
    bool allOK = true;
    int failures = 0;
    constexpr int NUM_TESTS = 5;

    std::mt19937 gen(98765);
    std::uniform_int_distribution<std::size_t> distIdx(0, allPoints.size() - 1);
    std::uniform_real_distribution<float> distRad(0.0f, 0.5f);

    for (int t = 0; t < NUM_TESTS; ++t) {
        const Point& centro = allPoints[distIdx(gen)];
        float radius = distRad(gen);
        Sphere sph(centro, radius);

        // SR-Tree
        std::vector<Point*> ptrs = tree.rangeQuery(sph);
        std::vector<Point> treeRes;
        treeRes.reserve(ptrs.size());
        for (Point* pptr : ptrs) {
            treeRes.push_back(*pptr);
        }

        // Fuerza bruta
        std::vector<Point> brute;
        for (const Point& p : allPoints) {
            if (pointInSphere(p, sph)) {
                brute.push_back(p);
            }
        }

        // Comparar 
        if (!samePointSet(treeRes, brute)) {
            failures++;
        }
    }

    if (failures == 0) {
        std::cout << "[OK] Test 4 (rangeQuery Sphere) pasó todos los subtests (" 
                  << NUM_TESTS << "/" << NUM_TESTS << ").\n";
    } else {
        std::cout << "[ERROR] Test 4 (rangeQuery Sphere): fallaron " 
                  << failures << " de " << NUM_TESTS << " subtests.\n";
        allOK = false;
    }
    return allOK;
}

// -------------------------------------------------------------
// TEST 5: k-NearestNeighbors
// -------------------------------------------------------------
bool testKNearestNeighbors(const SRTree& tree, const std::vector<Point>& allPoints) {
    bool allOK = true;
    int failures = 0;
    constexpr int NUM_TESTS = 5;

    std::mt19937 gen(19283);
    std::uniform_int_distribution<int> distK(1, 10);

    for (int t = 0; t < NUM_TESTS; ++t) {
        Point query = Point::random(0.0f, 1.0f);
        int k = distK(gen);

        // SR-Tree
        std::vector<Point*> treeNbrs = tree.kNearestNeighbors(query, static_cast<std::size_t>(k));

        // Fuerza bruta
        std::vector<std::pair<float, const Point*>> distList;
        distList.reserve(allPoints.size());
        for (const Point& p : allPoints) {
            float d = Point::distance(query, p);
            distList.emplace_back(d, &p);
        }
        std::sort(distList.begin(), distList.end(),
                  [](auto& a, auto& b) { return a.first < b.first; });

        std::vector<float> bruteDists;
        for (int i = 0; i < k; ++i) {
            bruteDists.push_back(distList[i].first);
        }

        // SR-Tree
        std::vector<float> treeDists;
        treeDists.reserve(treeNbrs.size());
        for (Point* pptr : treeNbrs) {
            treeDists.push_back(Point::distance(query, *pptr));
        }
        std::sort(treeDists.begin(), treeDists.end());

        if (!sameDistanceList(treeDists, bruteDists)) {
            failures++;
        }
    }

    if (failures == 0) {
        std::cout << "[OK] Test 5 (k-NearestNeighbors) pasó todos los subtests (" 
                  << NUM_TESTS << "/" << NUM_TESTS << ").\n";
    } else {
        std::cout << "[ERROR] Test 5 (k-NearestNeighbors): fallaron " 
                  << failures << " de " << NUM_TESTS << " subtests.\n";
        allOK = false;
    }
    return allOK;
}




int main() {
    bool overallOK = true;

    constexpr std::size_t NUM_POINTS  = 1000;
    constexpr std::size_t MAX_ENTRIES =   18;

    std::vector<Point> allPoints;
    allPoints.reserve(NUM_POINTS);
    std::mt19937 genPts(55555);

    for (std::size_t i = 0; i < NUM_POINTS; ++i) {
        allPoints.push_back(Point::random(0.0f, 1.0f));
    }

    SRTree tree(MAX_ENTRIES);
    for (const Point& p : allPoints) {
        tree.insert(p);
    }

    // Ejecutar los tests
    std::cout << "=== TEST 1: Bounding Volumes ===\n";
    if (!testBoundingVolumes(tree)) overallOK = false;

    std::cout << "\n=== TEST 2: Search ===\n";
    if (!testSearch(tree, allPoints)) overallOK = false;

    std::cout << "\n=== TEST 3: rangeQuery(MBB) ===\n";
    if (!testRangeQueryBox(tree, allPoints)) overallOK = false;

    std::cout << "\n=== TEST 4: rangeQuery(Sphere) ===\n";
    if (!testRangeQuerySphere(tree, allPoints)) overallOK = false;

    std::cout << "\n=== TEST 5: k-NearestNeighbors ===\n";
    if (!testKNearestNeighbors(tree, allPoints)) overallOK = false;

    if (overallOK) {
        std::cout << "\n>>> TODOS LOS TESTS PASARON!! <<<\nAun puedes aprobar :D\n";
        return 0;
    } else {
        std::cout << "\n>>> ALGÚN TEST FALLÓ <<<\nRevisa las fechas de retiro\n";
        return 1;
    }
}