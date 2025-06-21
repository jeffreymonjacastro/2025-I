#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "Mtree.h"
#include "Object.h"

std::string randomString(std::mt19937 &gen, std::size_t len = 9) {
  static const char alphabet[] =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789";
  std::uniform_int_distribution<> d(0, sizeof(alphabet) - 2);
  std::string s;
  s.reserve(len);
  for (std::size_t i = 0; i < len; ++i)
    s.push_back(alphabet[d(gen)]);
  return s;
}

void collectObjects(const MNode *node, std::vector<const Object *> &out) {
  if (node->isLeaf()) {
    for (const Object *o : node->objects())
      out.push_back(o);
  } else {
    for (const MNode *child : node->children())
      collectObjects(child, out);
  }
}

bool subtreeContains(const MNode *node, const Object *obj) {
  if (node->isLeaf()) {
    return std::find(node->objects().begin(), node->objects().end(), obj) != node->objects().end();
  }
  for (const MNode *child : node->children())
    if (subtreeContains(child, obj))
      return true;
  return false;
}

// -------------------------------------------------------------
// TEST 1: Verificar regiones
// -------------------------------------------------------------
bool verifyRegions(const MNode *node) {
  if (!node) {
    std::cerr << "[TEST1] Arbol vacio o nodo nulo\n";
    return false;
  }

  const Object *p = node->pivot();
  if (!p || !subtreeContains(node, p)) {
    std::cerr << "[TEST1] Pivot no pertenece al subárbol\n";
    return false;
  }

  std::vector<const Object *> objs;
  collectObjects(node, objs);

  // Verificar radio
  for (const Object *o : objs) {
    std::size_t dist = p->distance(o);
    if (dist > node->radius()) {
      std::cerr << "[TEST1] Objeto fuera del radio de cobertura\n";
      return false;
    }
  }

  // Verificar pivotDist
  const auto &pd = node->pivotDistances();
  if (node->isLeaf()) {
    if (pd.size() != node->objects().size()) {
      std::cerr << "[TEST1] pivotDist mal dimensionado en hoja\n";
      return false;
    }
    for (std::size_t i = 0; i < pd.size(); ++i)
      if (pd[i] != p->distance(node->objects()[i])) {
        std::cerr << "[TEST1] pivotDist incorrecto en hoja\n";
        return false;
      }
  } else {
    if (pd.size() != node->children().size()) {
      std::cerr << "[TEST1] pivotDist mal dimensionado en interno\n";
      return false;
    }
    for (std::size_t i = 0; i < pd.size(); ++i)
      if (pd[i] != p->distance(node->children()[i]->pivot())) {
        std::cerr << "[TEST1] pivotDist incorrecto en interno\n";
        return false;
      }
  }

  // Recursión
  for (const MNode *child : node->children())
    if (!verifyRegions(child))
      return false;

  return true;
}

// -------------------------------------------------------------
// TEST 2: Object search
// -------------------------------------------------------------
bool testSearch(const MTree &tree,
                const std::vector<std::unique_ptr<Object>> &data,
                std::mt19937 &gen) {
  // Todos los objetos insertados deben encontrarse
  for (const auto &ptr : data) {
    if (!tree.search(*ptr)) {
      std::cerr << "[TEST2] No encontro el objeto insertado: " << ptr->str() << "\n";
      return false;
    }
  }

  // Nuevos objetos no deben encontrarse
  std::unordered_set<std::string> dict;
  for (const auto &ptr : data)
    dict.insert(ptr->str());

  for (int i = 0; i < 30; ++i) {
    std::string s;

    // Asegurar no duplicado en data
    do {
      s = randomString(gen);
    } while (dict.count(s));

    Object tmp(s);
    if (tree.search(tmp)) {
      std::cerr << "[TEST2] Encontro string no insertado: " << s << "\n";
      return false;
    }
  }

  return true;
}

// -------------------------------------------------------------
// TEST 3: Radius search
// -------------------------------------------------------------
bool testRangeQuery(const MTree &tree,
                    const std::vector<std::unique_ptr<Object>> &data,
                    std::mt19937 &gen) {
  std::uniform_int_distribution<> distR(1, 3);
  std::uniform_int_distribution<> distIdx(0, data.size() - 1);

  for (int t = 0; t < 5; ++t) {
    const Object &query = *data[distIdx(gen)];
    std::size_t radius = distR(gen);

    // M-tree
    auto resTree = tree.rangeSearch(query, radius);
    std::unordered_set<const Object *> setTree(resTree.begin(), resTree.end());

    // Fuerza bruta
    std::vector<const Object *> brute;
    for (const auto &p : data)
      if (query.distance(p.get()) <= radius)
        brute.push_back(p.get());
    std::unordered_set<const Object *> setBrute(brute.begin(), brute.end());

    if (setTree != setBrute) {
      std::cerr << "[TEST3] Algo anda mal en radius=" << radius
                << " (tree=" << setTree.size()
                << ", brute=" << setBrute.size() << ")\n";
      return false;
    }
  }
  return true;
}

// -------------------------------------------------------------
// TEST 4: k-Nearest Neighbors
// -------------------------------------------------------------
bool testKNN(const MTree &tree,
             const std::vector<std::unique_ptr<Object>> &data,
             std::mt19937 &gen) {
  std::uniform_int_distribution<> distK(1, 5);
  std::uniform_int_distribution<> distIdx(0, data.size() - 1);

  for (int t = 0; t < 5; ++t) {
    const Object &query = *data[distIdx(gen)];
    std::size_t k = distK(gen);

    // kNN por M-tree
    auto resTree = tree.kNearestNeighbors(query, k);

    // Fuerza bruta
    std::vector<std::pair<std::size_t, const Object *>> all;
    for (const auto &p : data)
      all.emplace_back(query.distance(p.get()), p.get());
    std::partial_sort(all.begin(), all.begin() + k, all.end(),
                      [](auto &a, auto &b) { return a.first < b.first; });

    std::unordered_set<const Object *> brute;
    for (std::size_t i = 0; i < k; ++i)
      brute.insert(all[i].second);

    if (resTree.size() != k) {
      std::cerr << "[TEST4] k-NN retorno " << resTree.size()
                << "vecinos, se esperaba " << k << "\n";
      return false;
    }
    for (Object *o : resTree) {
      if (!brute.count(o)) {
        std::cerr << "[TEST4] Vecino incorrecto u.u: " << o->str() << "\n";
        return false;
      }
    }
  }
  return true;
}

int main() {
  std::mt19937 gen(42);
  const std::size_t N = 1000;
  const std::size_t maxEntries = 10;

  MTree tree(maxEntries);
  std::vector<std::unique_ptr<Object>> data;
  data.reserve(N);
  for (std::size_t i = 0; i < N; ++i) {
    auto s = std::make_unique<Object>(randomString(gen));
    tree.insert(*s);
    data.push_back(std::move(s));
  }

  bool ok1 = verifyRegions(tree.root());
  bool ok2 = testSearch(tree, data, gen);
  bool ok3 = testRangeQuery(tree, data, gen);
  bool ok4 = testKNN(tree, data, gen);

  std::cout << "TEST 1 (regiones).............. " << (ok1 ? "OK" : "FAIL") << '\n';
  std::cout << "TEST 2 (search)................ " << (ok2 ? "OK" : "FAIL") << '\n';
  std::cout << "TEST 3 (rangeQuery)............ " << (ok3 ? "OK" : "FAIL") << '\n';
  std::cout << "TEST 4 (k-NN).................. " << (ok4 ? "OK" : "FAIL") << '\n';

  if (ok1 && ok2 && ok3 && ok4)
    std::cout << "\nAun es posible aprobar el curso!\nSolo espero que Gradescope no detecte nada raro.\n";
  else
    std::cout << "\nEl tramite de retiro aun esta activo.\n";

  return (ok1 && ok2 && ok3 && ok4) ? 0 : 1;
}