#include "distance_point_to_line_3D.h"

int main() {
  double p1x = 0, p1y = 0, p1z = 0;
  double dx = 1, dy = 1, dz = 1;
  double px = 1, py = 2, pz = 2;

  cout << "Distance: "
       << distance_point_to_line_3D(p1x, p1y, p1z, dx, dy, dz, px, py, pz)
       << endl;
  return 0;
}