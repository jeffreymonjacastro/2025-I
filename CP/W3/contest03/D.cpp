#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

string s1, s2, s3;

long long convertir(const vector<int> &indices, const vector<int> &perm) {
  long long resultado = 0;
  for (int indice : indices) {
    resultado = resultado * 10 + perm[indice];
  }
  return resultado;
}

void solve() {
  cin >> s1 >> s2 >> s3;

  set<char> caracteres_unicos(s1.begin(), s1.end());
  caracteres_unicos.insert(s2.begin(), s2.end());
  caracteres_unicos.insert(s3.begin(), s3.end());

  if (caracteres_unicos.size() > 10) {
    cout << "UNSOLVABLE" << endl;
    return;
  }

  vector<char> lista_caracteres(caracteres_unicos.begin(),
                                caracteres_unicos.end());
  map<char, int> mapa_indices;
  for (int i = 0; i < lista_caracteres.size(); ++i) {
    mapa_indices[lista_caracteres[i]] = i;
  }

  vector<int> indices_s1, indices_s2, indices_s3;
  for (char c : s1)
    indices_s1.push_back(mapa_indices[c]);
  for (char c : s2)
    indices_s2.push_back(mapa_indices[c]);
  for (char c : s3)
    indices_s3.push_back(mapa_indices[c]);

  vector<int> permutacion(10);
  iota(permutacion.begin(), permutacion.end(), 0);

  do {
    vector<int> asignacion_actual(10, -1);
    for (int i = 0; i < lista_caracteres.size(); ++i) {
      asignacion_actual[i] = permutacion[i];
    }

    if (asignacion_actual[indices_s1[0]] == 0 ||
        asignacion_actual[indices_s2[0]] == 0 ||
        asignacion_actual[indices_s3[0]] == 0)
      continue;

    long long numero1 = convertir(indices_s1, asignacion_actual);
    long long numero2 = convertir(indices_s2, asignacion_actual);
    long long numero3 = convertir(indices_s3, asignacion_actual);

    if (numero1 + numero2 == numero3) {
      cout << numero1 << endl;
      cout << numero2 << endl;
      cout << numero3 << endl;
      return;
    }

    // Generar la siguiente permutación manualmente
    int k = -1, l = -1;
    for (int i = permutacion.size() - 2; i >= 0; --i) {
      if (permutacion[i] < permutacion[i + 1]) {
        k = i;
        break;
      }
    }
    if (k == -1)
      break; // No hay más permutaciones

    for (int i = permutacion.size() - 1; i > k; --i) {
      if (permutacion[i] > permutacion[k]) {
        l = i;
        break;
      }
    }
    swap(permutacion[k], permutacion[l]);
    reverse(permutacion.begin() + k + 1, permutacion.end());

  } while (true);

  cout << "UNSOLVABLE" << endl;
}

int main() {
  cpu();
  int casos = 1;
  // cin >> casos;
  while (casos--)
    solve();
  return 0;
}
