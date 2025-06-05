#include "solution.h"

int main() {
  double cx = 1.0;
  double cy = 1.0;
  double cz = 1.0;
  Point center(cx, cy, cz);
  sphere_with_texture("sphere-rectangles.ply", "texture.jpg", center, "sphere-with-texture.ply");

  cout << "Sphere generated" << endl;
  return 0;
}