#include <iostream>
#include <vector>

using namespace std;

float cross_product(float x1, float y1, float x2, float y2) {
  return x1 * y2 - x2 * y1;
}

int sgn(float num) { return (num > 0) - (num < 0); }

//* Intersection between two segments in 2D
bool segment_segment_intersects(float x1, float y1, float x2, float y2,
                                float x3, float y3, float x4, float y4) {

  float P1P2x = x2 - x1;
  float P1P2y = y2 - y1;

  float P1P3x = x3 - x1;
  float P1P3y = y3 - y1;

  float P1P2_cross_P1P3 = cross_product(P1P2x, P1P2y, P1P3x, P1P3y);
  int P1P2_cross_P1P3_sign = sgn(P1P2_cross_P1P3);

  float P1P4x = x4 - x1;
  float P1P4y = y4 - y1;

  float P1P2_cross_P1P4 = cross_product(P1P2x, P1P2y, P1P4x, P1P4y);
  int P1P2_cross_P1P4_sign = sgn(P1P2_cross_P1P4);

  //! Checks if both segments are colineals
  if (P1P2_cross_P1P3 == 0 && P1P2_cross_P1P4 == 0) {
    if (min(x1, x2) <= max(x3, x4) && max(x1, x2) >= min(x3, x4) &&
        min(y1, y2) <= max(y3, y4) && max(y1, y2) >= min(y3, y4)) {
      return true;
    }
    return false;
  }

  if (P1P2_cross_P1P3_sign == P1P2_cross_P1P4_sign)
    return false;

  float P3P4x = x4 - x3;
  float P3P4y = y4 - y3;

  float P3P1x = x1 - x3;
  float P3P1y = y1 - y3;

  float P3P4_cross_P3P1 = cross_product(P3P4x, P3P4y, P3P1x, P3P1y);
  int P3P4_cross_P3P1_sign = sgn(P3P4_cross_P3P1);

  float P3P2x = x2 - x3;
  float P3P2y = y2 - y3;

  float P3P4_cross_P3P2 = cross_product(P3P4x, P3P4y, P3P2x, P3P2y);
  int P3P4_cross_P3P2_sign = sgn(P3P4_cross_P3P2);

  if (P3P4_cross_P3P1_sign == P3P4_cross_P3P2_sign)
    return false;

  return true;
}

//* Distance of point to a Line in 3D
float distance_point_line(float lx, float ly, float lz, float dx, float dy,
                          float dz, float px, float py, float pz) {
  return 0;
}

int main() {
  // cout << segment_segment_intersects(1, 1, 2, 2, 2, 1, 1, 2) << endl;
  // cout << segment_segment_intersects(0, 1, 1, 0, 1, 1, 2, 2) << endl;
  cout << segment_segment_intersects(1, 1, 2, 2, 2, 2, 3, 3) << endl;
  return 0;
}