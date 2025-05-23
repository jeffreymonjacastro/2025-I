#include "PointGenerator.h"
#include "jarvis_graham.h"
#include <fstream>
#include <iomanip>

using namespace chrono;

int main() {
  ofstream file("other.md");
  vector<int> sizes = {1000, 10000, 100000, 1000000, 2000000, 5000000};
  vector<PointDistribution> distributions = {PointDistribution::RANDOM_POINT};
  // vector<PointDistribution> distributions = {
  // PointDistribution::RANDOM_CIRCLE,    PointDistribution::CIRCLE_BORDER,
  // PointDistribution::RANDOM_RECTANGLE, PointDistribution::RECTANGLE_BORDER,
  // PointDistribution::PARABOLA_REGION,  PointDistribution::PARABOLA_CURVE,
  // PointDistribution::RANDOM_POINT
  // };
  vector<bool> withInteriorPointsRemoval = {false, true};

  // Encabezado de la tabla
  file << "| Distribucion       | Tamano   | Jarvis (ms) | Jarvis con opt "
          "(ms) | Graham (ms) | Graham con opt (ms) |"
       << endl;
  file << "|--------------------|----------|-------------|-------------------"
          "--|-------------|---------------------|"
       << endl;

  for (auto dist : distributions) {
    for (int size : sizes) {
      auto points = generate_test_points<int>(size, dist);

      file << fixed << setprecision(2) << "|" << left << setw(20)
           << distribution_to_string(dist) << "|" << right << setw(9) << size
           << " |";
      for (bool withInterior : withInteriorPointsRemoval) {

        // Medir tiempo de Jarvis March
        auto start = high_resolution_clock::now();
        auto hull_jarvis = jarvis(points, withInterior);
        auto end = high_resolution_clock::now();
        duration<double, milli> jarvis_time = end - start;

        file << right << (withInterior ? setw(20) : setw(12))
             << jarvis_time.count() << " |";
      }

      for (bool withInterior : withInteriorPointsRemoval) {
        // Medir tiempo de Graham Scan
        auto start = high_resolution_clock::now();
        auto hull_graham = graham(points, withInterior);
        auto end = high_resolution_clock::now();
        duration<double, milli> graham_time = end - start;

        file << right << (withInterior ? setw(20) : setw(12))
             << graham_time.count() << " |";
      }

      file << endl;

      cout << "Distribucion: " << distribution_to_string(dist)
           << ", Tamano: " << size << endl;
    }
  }

  return 0;
}