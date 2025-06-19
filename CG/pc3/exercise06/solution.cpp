#include "solution.h"

int main() {
  rotate_mesh_around_line("cube.ply", 0, 0, 0, 0, 0, 1, 45, "cube_rotated.ply");

  cout << "Rotacion de malla PLY completada exitosamente." << endl;

  return 0;
}