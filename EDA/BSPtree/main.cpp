#include <iostream>
#include <random>
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "BSPTree.h"
#include "Ball.h"
#include "Plane.h"
#include "Point.h"
#include "DataType.h"
#include "Line.h"


// ---------------------------------------------------------------------
// Funciones auxiliares 
// ---------------------------------------------------------------------

void generateOrthonormalBasis(const Vector3D<NType>& normal, Vector3D<NType>& u, Vector3D<NType>& v) {
    Point3D<NType> arbitrary;
    if (std::abs(normal.getX().getValue()) < 0.9f)
        arbitrary = Point3D<NType>(NType(1), NType(0), NType(0));
    else
        arbitrary = Point3D<NType>(NType(0), NType(1), NType(0));
    u = normal.cross(arbitrary).normalized();
    v = normal.cross(u).normalized();
}


Polygon<NType> generateRandomPolygon(int minPoints = 3, int maxPoints = 5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Número de vértices
    std::uniform_int_distribution<> pointsDist(minPoints, maxPoints);
    int numPoints = pointsDist(gen);

    // Centro aleatorio
    std::uniform_real_distribution<float> centerDist(-100.0f, 100.0f);
    Point3D<NType> center(NType(centerDist(gen)), NType(centerDist(gen)), NType(centerDist(gen)));

    // Normal aleatoria
    std::uniform_real_distribution<float> angleDist(0.0f, 3.14159f);
    float theta = angleDist(gen);
    float phi = angleDist(gen);
    NType nx = NType(std::sin(theta) * std::cos(phi));
    NType ny = NType(std::sin(theta) * std::sin(phi));
    NType nz = NType(std::cos(theta));
    Vector3D<NType> normal(nx, ny, nz);
    normal.normalize();

    // Base ortonormal
    Vector3D<NType> u, v;
    generateOrthonormalBasis(normal, u, v);

    std::uniform_real_distribution<float> radiusDist(5.0f, 20.0f);

    // Generar angulos para generar los vertices
    std::vector<float> angles;
    std::uniform_real_distribution<float> offsetDist(0.0f, 2 * 3.14159f);
    float baseAngle = offsetDist(gen);
    float angleStep = 2 * 3.14159f / numPoints;
    for (int i = 0; i < numPoints; i++) {
        float angle = baseAngle - i * angleStep;
        angles.push_back(angle);
    }
    
    // Generar los vertices
    std::vector<Point3D<NType>> vertices;
    for (int i = 0; i < numPoints; i++) {
        float r = radiusDist(gen);
        float angle = angles[i];
        Point3D<NType> offset = (u * NType(r * std::cos(angle))) + (v * NType(r * std::sin(angle)));
        Point3D<NType> vertex = center + offset;
        vertices.push_back(vertex);
    }
    return Polygon<NType>(vertices);
}


// Mismos planos
bool similarPlane(const Plane<NType>& p1, const Plane<NType>& p2, float tol = 1e-3f) {
    // Obtener las normales
    Vector3D<NType> n1 = p1.getNormal();
    Vector3D<NType> n2 = p2.getNormal();
    
    NType dot = n1.dot(n2);
    if (!(dot >= NType(1.0f) || dot <= NType(-1.0f)))
        return false;
    
    if (dot <= NType(-1.0f)) {
        n2 = -n2;
    }
    
    NType d1 = -(n1.dot(p1.getPoint()));
    NType d2 = -(n2.dot(p2.getPoint()));
    
    return std::abs(d1.getValue() - d2.getValue()) < tol;
}


Ball<NType> generateRandomBall() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);
    Point3D<NType> pos(NType(posDist(gen)), NType(posDist(gen)), NType(posDist(gen)));
    
    std::uniform_real_distribution<float> velDist(-30.0f, 30.0f);
    Vector3D<NType> vel(NType(velDist(gen)), NType(velDist(gen)), NType(velDist(gen)));
    
    std::uniform_real_distribution<float> radiusDist(1.0f, 5.0f);
    NType radius = NType(radiusDist(gen));
    
    return Ball<NType>(pos, vel, radius);
}

// Intersección de un Ball en movimiento con un polígono
bool sweptSphereIntersectsPolygon(const Ball<NType>& ball, const LineSegment<NType>& movement, const Polygon<NType>& poly) {
    Plane<NType> plane = poly.getPlane();
    NType r = ball.getRadius();
    NType dStart = plane.distance(movement.getP1());
    NType dEnd   = plane.distance(movement.getP2());
    
    // Claro que no hay interseccion
    if ((dStart > r && dEnd > r) || (dStart < -r && dEnd < -r))
        return false;
    
    // Calcular t (si existe)
    NType denom = dStart - dEnd;
    if (denom == 0) return false; // Evitar división por cero.
    NType t = dStart / denom;
    if (t < NType(0) || t > NType(1))
        return false;
    
    // Punto de interseccion
    Point3D<NType> intersection = movement.getP1() + (movement.getP2() - movement.getP1()) * t;
    return poly.contains(intersection);
}

// Comparar dos conjuntos de polígonos
bool comparePolygonSets(const std::vector<Polygon<NType>>& a, const std::vector<Polygon<NType>>& b, float tol = 1e-3f) {
    auto findMatch = [&](const Polygon<NType>& poly) -> bool {
        for (const auto& cand : b) {
            if (std::abs(poly.area().getValue() - cand.area().getValue()) < tol &&
                ([](const Plane<NType>& p1, const Plane<NType>& p2, float tol2 = 1e-3f) -> bool {
                    Vector3D<NType> n1 = p1.getNormal(), n2 = p2.getNormal();
                    NType dot = n1.dot(n2);
                    if (!(dot >= NType(0.999f) || dot <= NType(-0.999f)))
                        return false;
                    Point3D<NType> pt1 = p1.getPoint(), pt2 = p2.getPoint();
                    return pt1.distance(pt2).getValue() < tol2;
                })(poly.getPlane(), cand.getPlane()))
            {
                return true;
            }
        }
        return false;
    };

    for (const auto& poly : a) {
        if (!findMatch(poly)) {
            std::cerr << "No match found for polygon with area " << poly.area().getValue() << "\n";
            return false;
        }
    }
    return true;
}

bool checkSubtreeValidity(const BSPNode<NType>* node, const Plane<NType>& parentPlane, bool isFront) {
    std::vector<Polygon<NType>> polys;
    node->collectPolygons(polys);
    for (const auto& poly : polys) {
        std::vector<Point3D<NType>> vertices = poly.getVertices();
        for (const auto& v : vertices) {
            NType d = parentPlane.distance(v);
            if (isFront) {
                if (d.getValue() < -0.1f)
                    return false;
            } else {
                if (d.getValue() > 0.1f)
                    return false;
            }
        }
    }
    return true;
}



// ---------------------------------------------------------------------
// Test 1: Verificar insercion
// ---------------------------------------------------------------------
void testPolygonsIntegrity() {
    std::cout << "Iniciando verificacion de insercion...\n";
    const int numPolygons = 20;
    std::vector<Polygon<NType>> originalPolys;
    BSPTree<NType> tree;
    
    // Insertar polígonos 
    for (int i = 0; i < numPolygons; ++i) {
        Polygon<NType> poly = generateRandomPolygon(3, 5);
        originalPolys.push_back(poly);
        tree.insert(poly);
    }
    
    // Obtener todos los polígonos del arbol
    auto candidatePolys = tree.getAllPolygons();
    
    // Siempre el numero de poligonos en el arbol es mayor o igual al de los originales
    assert(candidatePolys.size() >= originalPolys.size());
    
    // Si los poligonos son divididos, la suma del area de los pedazos debe ser igual al area del poligono original
    float  totalOriginalArea = 0.0f;
    float totalCandidateArea = 0.0f;
    for (const auto& orig : originalPolys) {
        totalOriginalArea += orig.area().getValue();
        
        float sumCandidateArea = 0.0f;
        // Buscar los que estan en el mismo plano
        for (const auto& cand : candidatePolys) {
            if (similarPlane(orig.getPlane(), cand.getPlane())) {
                sumCandidateArea += cand.area().getValue();
            }
        }
        float diff = std::abs(sumCandidateArea - orig.area().getValue());
        assert(diff < 1e-1f);
        totalCandidateArea += sumCandidateArea;
    }
    
    // Tambien verificamos en todos los poligonos
    float totalDiff = std::abs(totalCandidateArea - totalOriginalArea);
    assert(totalDiff < 1e-1f);
    
    std::cout << "Test de integridad de polígonos pasó exitosamente.\n";
}


// ---------------------------------------------------------------------
// Test 2: Query
// ---------------------------------------------------------------------
void testQueryRandomBalls(int iterations = 40) {
    std::cout << "Iniciando test de query con Balls...\n";
    
    BSPTree<> tree;
    const int numPolygons = 100;
    std::vector<Polygon<>> originalPolys;
    for (int i = 0; i < numPolygons; ++i) {
        Polygon<> poly = generateRandomPolygon(3, 5);
        originalPolys.push_back(poly);
        tree.insert(poly);
    }
    
    std::vector<Polygon<>> allPolys = tree.getAllPolygons();
    for (int i = 0; i < iterations; ++i) {
        Ball<> ball = generateRandomBall();
        float dt = 2.0f;
        LineSegment<> movement = ball.step(dt);
        
        // Brute force
        std::vector<Polygon<>> bruteCandidates;
        for (const auto& poly : allPolys) {
            if (sweptSphereIntersectsPolygon(ball, movement, poly))
                bruteCandidates.push_back(poly);
        }
        
        // Usar el query
        std::vector<Polygon<>> queryCandidates = tree.query(ball, movement);
        
        // Ordenar
        auto sortByArea = [](const Polygon<>& a, const Polygon<>& b) {
            return a.area().getValue() < b.area().getValue();
        };
        std::sort(bruteCandidates.begin(), bruteCandidates.end(), sortByArea);
        std::sort(queryCandidates.begin(), queryCandidates.end(), sortByArea);
        
        // Comparar
        bool match = comparePolygonSets(bruteCandidates, queryCandidates) &&
                     comparePolygonSets(queryCandidates, bruteCandidates);
        std::cout << "Iteration " << i << ": " 
                  <<   "Brute = " << bruteCandidates.size() 
                  << ", Query = " << queryCandidates.size() << "\n";
        assert(match && "No se obtienen los mismos resultado al aplicar fuerza bruta y al emplear tu implementacion de query BSP.");
    }
    
    std::cout << "Test de query con Balls aleatorias pasó exitosamente.\n";
}


// ---------------------------------------------------------------------
// Test 3: Test de estructura del árbol (recorrido y consistencia)
// ---------------------------------------------------------------------
void testTreeStructureValidity() {
    std::cout << "Iniciando test de validez del arbol...\n";
    BSPTree<NType> tree;
    const int numPolygons = 50;
    for (int i = 0; i < numPolygons; ++i) {
        Polygon<NType> poly = generateRandomPolygon(3, 5);
        tree.insert(poly);
    }
    
    auto nodes = tree.getAllNodes();
    assert(!nodes.empty());
    
    // Verificar que el arbol este correctamente construido
    // Los poligonos en el nodo front deben estar delante del plano
    // y los del nodo back deben estar detras del plano
    for (const BSPNode<NType>* node : nodes) {

        const BSPNode<NType>* frontNode = node->getFront();
        if (frontNode) {
            bool validFront = checkSubtreeValidity(frontNode, node->getPartition(), true);
            assert(validFront && "El subárbol front contiene polígonos que no están delante del plano del nodo.");
        }

        const BSPNode<NType>* backNode = node->getBack();
        if (backNode) {
            bool validBack = checkSubtreeValidity(backNode, node->getPartition(), false);
            assert(validBack && "El subárbol back contiene polígonos que no están detrás del plano del nodo.");
        }
    }
    
    std::cout << "Test de estructura y validez del árbol pasó exitosamente.\n";
}

int main() {
    testTreeStructureValidity();
    testPolygonsIntegrity();
    testQueryRandomBalls();
    
    std::cout << "\nTodos los tests se ejecutaron correctamente.\n";
    return 0;
}