#include "solution.h"

int main() {
  double cx = 1.0;
  double cy = 1.0;
  double cz = 1.0;
  double R = 0.5;
  sphere_with_triangular_faces("sphere-triangles.ply", R, cx, cy, cz);

  cout << "Sphere generated" << endl;
  return 0;
}