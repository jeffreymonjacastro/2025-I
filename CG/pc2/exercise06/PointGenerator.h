#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

// Enum para los tipos de distribución
enum class PointDistribution {
  RANDOM_CIRCLE,    // Puntos aleatorios dentro de un círculo
  CIRCLE_BORDER,    // Puntos aleatorios en el borde de un círculo
  RANDOM_RECTANGLE, // Puntos aleatorios dentro de un rectángulo
  RECTANGLE_BORDER, // Puntos aleatorios en el borde de un rectángulo
  PARABOLA_REGION,  // Puntos aleatorios dentro de región limitada por parábola
  PARABOLA_CURVE,   // Puntos aleatorios sobre la parábola
  RANDOM_POINT      // Puntos aleatorios
};

template <typename T> class PointGenerator {
private:
  mutable mt19937 gen;

public:
  PointGenerator()
      : gen(chrono::steady_clock::now().time_since_epoch().count()) {}

  // Función principal para generar puntos
  vector<vector<T>> generate_points(int num_points,
                                    PointDistribution distribution,
                                    T scale = 1000) const {
    vector<vector<T>> points;
    points.reserve(num_points);

    switch (distribution) {
    case PointDistribution::RANDOM_CIRCLE:
      return generate_random_circle(num_points, scale);
    case PointDistribution::CIRCLE_BORDER:
      return generate_circle_border(num_points, scale);
    case PointDistribution::RANDOM_RECTANGLE:
      return generate_random_rectangle(num_points, scale);
    case PointDistribution::RECTANGLE_BORDER:
      return generate_rectangle_border(num_points, scale);
    case PointDistribution::PARABOLA_REGION:
      return generate_parabola_region(num_points, scale);
    case PointDistribution::PARABOLA_CURVE:
      return generate_parabola_curve(num_points, scale);
    case PointDistribution::RANDOM_POINT:
      return generate_random_point(num_points, scale);
    default:
      return generate_random_rectangle(num_points, scale);
    }
  }

private:
  // 1. Puntos aleatorios dentro de un círculo
  vector<vector<T>> generate_random_circle(int num_points, T radius) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> angle_dist(0, 2 * M_PI);
    uniform_real_distribution<double> radius_dist(0, 1);

    for (int i = 0; i < num_points; i++) {
      double angle = angle_dist(gen);
      double r = sqrt(radius_dist(gen)) * radius;

      T x = static_cast<T>(r * cos(angle));
      T y = static_cast<T>(r * sin(angle));
      points.push_back({x, y});
    }

    return points;
  }

  // 2. Puntos aleatorios en el borde de un círculo
  vector<vector<T>> generate_circle_border(int num_points, T radius) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> angle_dist(0, 2 * M_PI);

    for (int i = 0; i < num_points; i++) {
      double angle = angle_dist(gen);

      T x = static_cast<T>(radius * cos(angle));
      T y = static_cast<T>(radius * sin(angle));
      points.push_back({x, y});
    }

    return points;
  }

  // 3. Puntos aleatorios dentro de un rectángulo
  vector<vector<T>> generate_random_rectangle(int num_points, T scale) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> x_dist(-scale, scale);
    uniform_real_distribution<double> y_dist(-scale, scale);

    for (int i = 0; i < num_points; i++) {
      T x = static_cast<T>(x_dist(gen));
      T y = static_cast<T>(y_dist(gen));
      points.push_back({x, y});
    }

    return points;
  }

  // 4. Puntos aleatorios en el borde de un rectángulo
  vector<vector<T>> generate_rectangle_border(int num_points, T scale) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> side_dist(0, 1);
    uniform_real_distribution<double> pos_dist(-scale, scale);

    for (int i = 0; i < num_points; i++) {
      double side = side_dist(gen);
      T x, y;

      if (side < 0.25) {
        x = static_cast<T>(pos_dist(gen));
        y = scale;
      } else if (side < 0.5) {
        x = scale;
        y = static_cast<T>(pos_dist(gen));
      } else if (side < 0.75) {
        x = static_cast<T>(pos_dist(gen));
        y = -scale;
      } else {
        x = -scale;
        y = static_cast<T>(pos_dist(gen));
      }

      points.push_back({x, y});
    }

    return points;
  }

  // 5. Puntos aleatorios dentro de región limitada por parábola
  vector<vector<T>> generate_parabola_region(int num_points, T scale) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> x_dist(-scale, scale);
    uniform_real_distribution<double> y_dist(0, scale * scale / 100);

    for (int i = 0; i < num_points; i++) {
      T x, y;
      bool valid = false;

      while (!valid) {
        x = static_cast<T>(x_dist(gen));
        y = static_cast<T>(y_dist(gen));

        double parabola_y = (x * x) / 100.0;
        if (y <= parabola_y) {
          valid = true;
        }
      }

      points.push_back({x, y});
    }

    return points;
  }

  // 6. Puntos aleatorios sobre la parábola
  vector<vector<T>> generate_parabola_curve(int num_points, T scale) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> x_dist(-scale, scale);

    for (int i = 0; i < num_points; i++) {
      T x = static_cast<T>(x_dist(gen));
      T y = static_cast<T>((x * x) / 100.0);

      points.push_back({x, y});
    }

    return points;
  }

  // 7. Puntos aleatorios
  vector<vector<T>> generate_random_point(int num_points, T scale) const {
    vector<vector<T>> points;
    uniform_real_distribution<double> x_dist(-scale, scale);
    uniform_real_distribution<double> y_dist(-scale, scale);

    for (int i = 0; i < num_points; i++) {
      T x = static_cast<T>(x_dist(gen));
      T y = static_cast<T>(y_dist(gen));
      points.push_back({x, y});
    }

    return points;
  }
};

// Función de conveniencia para crear el generador y generar puntos
template <typename T>
vector<vector<T>> generate_test_points(int num_points,
                                       PointDistribution distribution,
                                       T scale = 1000) {
  PointGenerator<T> generator;
  return generator.generate_points(num_points, distribution, scale);
}

// Función para convertir enum a string (para logging)
string distribution_to_string(PointDistribution dist) {
  switch (dist) {
  case PointDistribution::RANDOM_CIRCLE:
    return "Random Circle";
  case PointDistribution::CIRCLE_BORDER:
    return "Circle Border";
  case PointDistribution::RANDOM_RECTANGLE:
    return "Random Rectangle";
  case PointDistribution::RECTANGLE_BORDER:
    return "Rectangle Border";
  case PointDistribution::PARABOLA_REGION:
    return "Parabola Region";
  case PointDistribution::PARABOLA_CURVE:
    return "Parabola Curve";
  case PointDistribution::RANDOM_POINT:
    return "Random Point";
  default:
    return "Unknown";
  }
}
