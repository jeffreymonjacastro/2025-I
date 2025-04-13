#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>
using namespace std;

long long convertir(const vector<int> &indices,
                    const vector<int> &permutation) {
  long long valor = 0;
  for (int i : indices) {
    valor = valor * 10 + permutation[i];
  }
  return valor;
}

int main() {
  string s1, s2, s3;
  cin >> s1 >> s2 >> s3;

  set<char> characters;
  for (char c : s1)
    characters.insert(c);
  for (char c : s2)
    characters.insert(c);
  for (char c : s3)
    characters.insert(c);

  if (characters.size() > 10) {
    cout << "UNSOLVABLE" << endl;
    return 0;
  }

  map<char, int> m;
  int i = 0;
  for (char c : characters) {
    m[c] = i++;
  }

  vector<int> S1, S2, S3;
  for (char c : s1)
    S1.push_back(m[c]);
  for (char c : s2)
    S2.push_back(m[c]);
  for (char c : s3)
    S3.push_back(m[c]);

  vector<int> permutation = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  do {
    if (permutation[S1[0]] == 0 || permutation[S2[0]] == 0 ||
        permutation[S3[0]] == 0)
      continue;

    long long n1 = convertir(S1, permutation);
    long long n2 = convertir(S2, permutation);
    long long n3 = convertir(S3, permutation);

    if (n1 + n2 == n3) {
      cout << n1 << endl;
      cout << n2 << endl;
      cout << n3 << endl;
      return 0;
    }
  } while (next_permutation(permutation.begin(), permutation.end()));

  cout << "UNSOLVABLE" << endl;
  return 0;
}