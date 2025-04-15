#include <iostream>
#include <vector>

using namespace std;

vector<float> cross_product(float Ax, float Ay, float Az, float Bx, float By,
                            float Bz) {
  float x = Ay * Bz - Az * By;
  float y = Az * Bx - Ax * Bz;
  float z = Ax * By - Ay * Bx;
  return {x, y, z};
}

//* Distance of point to a Line in 3D
float distance_point_line(float lx, float ly, float lz, float dx, float dy,
                          float dz, float px, float py, float pz) {
  return 0;
}

int main() { return 0; }