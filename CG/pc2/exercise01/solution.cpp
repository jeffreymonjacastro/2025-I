#include "intersection.h"

using namespace std;

int main() {
  cout << boolalpha;
  cout << intersection(1, 1, 2, 2, 3, 3, 4, 4) << endl;
  cout << intersection(1.0, 1.0, 2.0, 2.0, 3.0, 3.0, 4.0, 4.0) << endl;
  return 0;
}