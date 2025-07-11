#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <sstream>
#include <type_traits>
#include <climits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ll long long
#define ld long double

using namespace std;

template <typename T>
class Vector {
private:
    bool is_int;

public:
    T x_, y_, z_;

    Vector() : x_(T{}), y_(T{}), z_(T{}) {
        is_int = is_same<int, T>::value;
    }

    Vector(double x, double y, double z) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (x > INT_MAX) ? INT_MAX : ((x < INT_MIN) ? INT_MIN : static_cast<T>(x));
            y_ = (y > INT_MAX) ? INT_MAX : ((y < INT_MIN) ? INT_MIN : static_cast<T>(y));
            z_ = (z > INT_MAX) ? INT_MAX : ((z < INT_MIN) ? INT_MIN : static_cast<T>(z));
        } else {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        }
    }
    
 
    template<typename U>
    Vector(const Vector<U>& other) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (other.x_ > INT_MAX) ? INT_MAX : ((other.x_ < INT_MIN) ? INT_MIN : static_cast<T>(other.x_));
            y_ = (other.y_ > INT_MAX) ? INT_MAX : ((other.y_ < INT_MIN) ? INT_MIN : static_cast<T>(other.y_));
            z_ = (other.z_ > INT_MAX) ? INT_MAX : ((other.z_ < INT_MIN) ? INT_MIN : static_cast<T>(other.z_));
        } else {
            x_ = static_cast<T>(other.x_);
            y_ = static_cast<T>(other.y_);
            z_ = static_cast<T>(other.z_);
        }
    }


    template<typename U>
    Vector<T>& operator=(const Vector<U>& other) {
        if (this == (void*)&other) return *this;
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (other.x_ > INT_MAX) ? INT_MAX : ((other.x_ < INT_MIN) ? INT_MIN : static_cast<T>(other.x_));
            y_ = (other.y_ > INT_MAX) ? INT_MAX : ((other.y_ < INT_MIN) ? INT_MIN : static_cast<T>(other.y_));
            z_ = (other.z_ > INT_MAX) ? INT_MAX : ((other.z_ < INT_MIN) ? INT_MIN : static_cast<T>(other.z_));
        } else {
            x_ = static_cast<T>(other.x_);
            y_ = static_cast<T>(other.y_);
            z_ = static_cast<T>(other.z_);
        }
        return *this;
    }

    [[nodiscard]] ld dot_product(const Vector<T>& other) const {
        if (is_int) {
            ll a = (ll)other.x_ * (ll)x_;
            ll b = (ll)other.y_ * (ll)y_;
            ll c = (ll)other.z_ * (ll)z_;
            return a + b + c;
        }
        ld a = (ld)other.x_ * (ld)x_;
        ld b = (ld)other.y_ * (ld)y_;
        ld c = (ld)other.z_ * (ld)z_;
        return a + b + c;
    }

    Vector<ld> cross_product(const Vector<T>& other) const {
        if (is_int) {
            ll x = (ll)y_ * (ll)other.z_ - (ll)z_ * (ll)other.y_;
            ll y = (ll)z_ * (ll)other.x_ - (ll)x_ * (ll)other.z_; 
            ll z = (ll)x_ * (ll)other.y_ - (ll)y_ * (ll)other.x_;
            return Vector<ld>((ld)x, (ld)y, (ld)z);
        }
        ld x = (ld)y_ * (ld)other.z_ - (ld)z_ * (ld)other.y_;
        ld y = (ld)z_ * (ld)other.x_ - (ld)x_ * (ld)other.z_; 
        ld z = (ld)x_ * (ld)other.y_ - (ld)y_ * (ld)other.x_;
        return Vector<ld>(x, y, z);
    }

    Vector<ld> operator/(const ld escalar)const {
        return Vector<ld>((ld)x_/escalar, (ld)y_/escalar, (ld)z_/escalar);
    }

    Vector<ld> operator*(const ld escalar)const {
        return Vector<ld>((ld)x_*escalar, (ld)y_*escalar, (ld)z_*escalar);
    }

    Vector<ld> operator+(const Vector<T>& other) const {
        return Vector<ld>((ld)x_ + (ld)other.x_, (ld)y_ + (ld)other.y_, (ld)z_ + (ld)other.z_);
    }

    Vector<ld> operator-(const Vector<T>& other) const {
        return Vector<ld>((ld)x_ - (ld)other.x_, (ld)y_ - (ld)other.y_, (ld)z_ - (ld)other.z_);
    }

    bool operator==(const Vector<T>& other) const {
        return x_ == other.x_ and y_ == other.y_ and z_ == other.z_;
    }

    ld norm() const {
        return sqrt(dot_product(*this));
    }

    Vector<ld> get_unit_vec() const {
        ld magnitude = this->norm();
        if(magnitude == 0) {
            return Vector<ld>(0.0, 0.0, 0.0);
        }
        return *this / magnitude;
    }
};

using Vec3 = Vector<double>;

struct Triangle {
    Vec3 v[3];
};

struct DrawableTriangle {
    Triangle tri;
    Vec3 normal;
    double max_depth;

    bool operator<(const DrawableTriangle& other) const {
        return max_depth > other.max_depth;
    }
};

bool load_ply(const string& filename, vector<Triangle>& triangles) {
    ifstream file(filename);
    if (not file.is_open()) return false;

    string line;
    long vertex_count = 0, face_count = 0;
    
    while (getline(file, line) and line != "end_header") {
        stringstream ss(line);
        string keyword;
        ss >> keyword;
        if (keyword == "element") {
            string type;
            ss >> type;
            if (type == "vertex") ss >> vertex_count;
            else if (type == "face") ss >> face_count;
        }
    }

    if (vertex_count == 0 or face_count == 0) return false;

    vector<Vec3> vertices(vertex_count);
    for (long i = 0; i < vertex_count; ++i) {
        file >> vertices[i].x_ >> vertices[i].y_ >> vertices[i].z_;
    }
    
    triangles.clear();
    for (long i = 0; i < face_count; ++i) {
        int num_verts;
        file >> num_verts;
        if (num_verts == 3) {
            int i0, i1, i2;
            file >> i0 >> i1 >> i2;
            if (i0 < vertex_count and i1 < vertex_count and i2 < vertex_count) {
                triangles.push_back({vertices[i0], vertices[i1], vertices[i2]});
            }
        } else {
            getline(file, line);
        }
    }
    return true;
}

void rasterize_triangle(vector<unsigned char>& buffer, size_t width, size_t height, 
                        const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& color) {
    int minX = max(0, (int)floor(min({p0.x_, p1.x_, p2.x_})));
    int maxX = min((int)width - 1, (int)ceil(max({p0.x_, p1.x_, p2.x_})));
    int minY = max(0, (int)floor(min({p0.y_, p1.y_, p2.y_})));
    int maxY = min((int)height - 1, (int)ceil(max({p0.y_, p1.y_, p2.y_})));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vec3 p((double)x + 0.5, (double)y + 0.5, 0);
            
            double area = (p1.y_ - p2.y_) * (p0.x_ - p2.x_) + (p2.x_ - p1.x_) * (p0.y_ - p2.y_);
            if (abs(area) < 1e-9) continue;

            double w0 = ((p1.y_ - p2.y_) * (p.x_ - p2.x_) + (p2.x_ - p1.x_) * (p.y_ - p2.y_)) / area;
            double w1 = ((p2.y_ - p0.y_) * (p.x_ - p2.x_) + (p0.x_ - p2.x_) * (p.y_ - p2.y_)) / area;
            double w2 = 1.0 - w0 - w1;

            if (w0 >= 0 and w1 >= 0 and w2 >= 0) {
                size_t index = (y * width + x) * 3;
                buffer[index]     = static_cast<unsigned char>(color.x_);
                buffer[index + 1] = static_cast<unsigned char>(color.y_);
                buffer[index + 2] = static_cast<unsigned char>(color.z_);
            }
        }
    }
}

void painter_algorithm_simple_cosine_illumination(
    string const& full_path_input_mesh,
    string const& full_path_output_image,
    double min_x_coordinate_in_projection_plane,
    double min_y_coordinate_in_projection_plane,
    double max_x_coordinate_in_projection_plane,
    double max_y_coordinate_in_projection_plane,
    size_t width_in_pixels,
    size_t height_in_pixels
) {
    vector<Triangle> mesh_triangles;
    if (not load_ply(full_path_input_mesh, mesh_triangles)) {
        cerr << "Error: No se pudo cargar el archivo de malla: " << full_path_input_mesh << endl;
        return;
    }

    vector<DrawableTriangle> drawable_triangles;
    for (const auto& tri : mesh_triangles) {
        DrawableTriangle dt;
        dt.tri = tri;
        dt.normal = (tri.v[2] - tri.v[0]).cross_product(tri.v[1] - tri.v[0]).get_unit_vec();
        dt.max_depth = max({tri.v[0].norm(), tri.v[1].norm(), tri.v[2].norm()});
        drawable_triangles.push_back(dt);
    }
    
    sort(drawable_triangles.begin(), drawable_triangles.end());

    vector<unsigned char> image_buffer(width_in_pixels * height_in_pixels * 3, 0);
    Vec3 vision_vec_opposite(0, 0, -1);

    for (const auto& dt : drawable_triangles) {
        double cos_alpha = dt.normal.dot_product(vision_vec_opposite);
        
        if (cos_alpha <= 0) continue;

        Vec3 color(255.0 * cos_alpha, 255.0 * cos_alpha, 255.0 * cos_alpha);
        
        Vec3 projected_points[3];
        bool triangle_is_valid = true;
        for (int i = 0; i < 3; ++i) {
            const auto& v = dt.tri.v[i];
            if (v.z_ <= 0.1) { 
                triangle_is_valid = false;
                break;
            }

            double proj_x = v.x_ / v.z_;
            double proj_y = v.y_ / v.z_;

            double screen_x = width_in_pixels * (proj_x - min_x_coordinate_in_projection_plane) / (max_x_coordinate_in_projection_plane - min_x_coordinate_in_projection_plane);
            double screen_y = height_in_pixels * (1.0 - (proj_y - min_y_coordinate_in_projection_plane) / (max_y_coordinate_in_projection_plane - min_y_coordinate_in_projection_plane));
            
            projected_points[i] = Vec3(screen_x, screen_y, 0);
        }

        if (triangle_is_valid) {
            rasterize_triangle(image_buffer, width_in_pixels, height_in_pixels, projected_points[0], projected_points[1], projected_points[2], color);
        }
    }
    
    stbi_write_png(full_path_output_image.c_str(), width_in_pixels, height_in_pixels, 3, image_buffer.data(), width_in_pixels * 3);
}

int main() {
    try {
        cout << "Generando imagen con Painter's Algorithm..." << endl;
        painter_algorithm_simple_cosine_illumination(
            "sphere.ply",
            "photo-of-sphere.png",
            -2.0,
            -2.0,
            2.0,
            2.0,
            400,
            400
        );
        cout << "Archivo 'photo-of-sphere.png' generado exitosamente." << endl;
    } catch (const exception& e) {
        cerr << "Ha ocurrido un error fatal: " << e.what() << endl;
        return 1;
    }
    return 0;
}