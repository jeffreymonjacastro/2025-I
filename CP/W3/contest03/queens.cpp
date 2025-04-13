#include <iostream>
#include <vector>

using namespace std;
/// genera(i) genera

// void imprimir(vector<int> y, int n, int conta) {
//   char c = '.', p = '*';
//   cout << conta << "\n";
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       if (y[i] == j)
//         cout << p;
//       else
//         cout << c;
//     }
//     cout << "\n";
//   }
//   cout << "\n\n";
// }

void generar(vector<int> &y, int i, int n, vector<bool> &col,
             vector<bool> &dia1, vector<bool> &dia2, int &conta) {
  if (i == n) {
    conta++;
  } else {
    for (int j = 0; j < n; j++) {

      if (col[j] || dia1[i + j] || dia2[i + (n - 1) - j])
        continue;
      y.push_back(j);
      col[j] = dia1[i + j] = dia2[i + (n - 1) - j] = true;
      generar(y, i + 1, n, col, dia1, dia2, conta);
      col[j] = dia1[i + j] = dia2[i + (n - 1) - j] = false;
      y.pop_back();
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  vector<int> y;
  vector<bool> col(8, false);
  vector<bool> dia1(15, false);
  vector<bool> dia2(15, false);
  int n;
  int conta = 0;
  cin >> n;
  generar(y, 0, n, col, dia1, dia2, conta);
  cout << conta << "\n";
  return 0;
}
