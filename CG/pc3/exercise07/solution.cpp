#include "solution.h"

int main() {
  translate_mesh("sphere-rectangles.ply", 0, 1, 0, "sphere-translated.ply");
  cout << "Translation completed successfully." << endl;
  return 0;
}