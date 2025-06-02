#ifndef POINT_H
#define POINT_H

#include <array>
#include <cmath>
#include <stdexcept>
#include <random>

constexpr std::size_t DIM = 768;
constexpr float EPSILON = 1e-8f;

class Point {
public:
    Point();
    explicit Point(const std::array<float, DIM>& coordinates);
    
    Point  operator+ (const Point& other) const;
    Point& operator+=(const Point& other);
    Point  operator- (const Point& other) const;
    Point& operator-=(const Point& other);
    Point  operator* (float scalar) const;
    Point& operator*=(float scalar);
    Point  operator/ (float scalar) const;
    Point& operator/=(float scalar);
    float norm() const;

    float  operator[](std::size_t index) const; 
    float& operator[](std::size_t index);

    static Point random(float min = 0.0f, float max = 1.0f);
    static float distance(const Point& p1, const Point& p2);

private:
    std::array<float, DIM> coordinates_;
};


Point::Point() {
    coordinates_.fill(0.0f);
}
Point::Point(const std::array<float, DIM>& coordinates) : coordinates_(coordinates) {}


Point Point::operator+(const Point& other) const {
    Point result;
    for (std::size_t i = 0; i < DIM; ++i) {
        result.coordinates_[i] = coordinates_[i] + other.coordinates_[i];
    }
    return result;
}
Point& Point::operator+=(const Point& other) {
    for (std::size_t i = 0; i < DIM; ++i) {
        coordinates_[i] += other.coordinates_[i];
    }
    return *this;
}


Point Point::operator-(const Point& other) const {
    Point result;
    for (std::size_t i = 0; i < DIM; ++i) {
        result.coordinates_[i] = coordinates_[i] - other.coordinates_[i];
    }
    return result;
}
Point& Point::operator-=(const Point& other) {
    for (std::size_t i = 0; i < DIM; ++i) {
        coordinates_[i] -= other.coordinates_[i];
    }
    return *this;
}


Point Point::operator*(float scalar) const {
    Point result;
    for (std::size_t i = 0; i < DIM; ++i) {
        result.coordinates_[i] = coordinates_[i] * scalar;
    }
    return result;
}
Point& Point::operator*=(float scalar) {
    for (std::size_t i = 0; i < DIM; ++i) {
        coordinates_[i] *= scalar;
    }
    return *this;
}


Point Point::operator/(float scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::invalid_argument("Division by zero");
    }
    Point result;
    for (std::size_t i = 0; i < DIM; ++i) {
        result.coordinates_[i] = coordinates_[i] / scalar;
    }
    return result;
}
Point& Point::operator/=(float scalar) {
    if (std::abs(scalar) < EPSILON) {
        throw std::invalid_argument("Division by zero");
    }
    for (std::size_t i = 0; i < DIM; ++i) {
        coordinates_[i] /= scalar;
    }
    return *this;
}


float Point::norm() const {
    float sum = 0.0f;
    for (std::size_t i = 0; i < DIM; ++i) {
        sum += coordinates_[i] * coordinates_[i];
    }
    return std::sqrt(sum);
}



float Point::operator[](std::size_t index) const {
    if (index >= DIM) {
        throw std::out_of_range("Index out of range");
    }
    return coordinates_[index];
}
float& Point::operator[](std::size_t index) {
    if (index >= DIM) {
        throw std::out_of_range("Index out of range");
    }
    return coordinates_[index];
}


static std::mt19937& global_engine() {
    static std::random_device rd;
    static std::mt19937 eng(rd());
    return eng;
}
Point Point::random(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    std::array<float, DIM> coords;
    for (auto& c : coords) c = dis(global_engine());
    return Point(coords);
}


float Point::distance(const Point& p1, const Point& p2) {
    float sum = 0.0f;
    for (std::size_t i = 0; i < DIM; ++i) {
        float diff = p1.coordinates_[i] - p2.coordinates_[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}


#endif // POINT_H
