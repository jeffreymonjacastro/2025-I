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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
struct Vec2 { double s = 0, t = 0; };

struct Vertex {
    Vec3 pos;
    Vec2 tex;
};

struct Triangle {
    Vertex v[3];
};

struct DrawableTriangle {
    Triangle tri;
    Vec3 normal;
    double max_depth;
    bool operator<(const DrawableTriangle& other) const { return max_depth > other.max_depth; }
};

struct Texture {
    int width = 0, height = 0, channels = 0;
    vector<unsigned char> data;
};

bool load_texture(const string& filename, Texture& texture) {
    unsigned char* raw_data = stbi_load(filename.c_str(), &texture.width, &texture.height, &texture.channels, 0);
    if (not raw_data) return false;
    texture.data.assign(raw_data, raw_data + texture.width * texture.height * texture.channels);
    stbi_image_free(raw_data);
    return true;
}

bool load_ply_with_texture(const string& filename, vector<Triangle>& triangles) {
    ifstream file(filename);
    if (not file.is_open()) return false;
    string line;
    long vertex_count = 0, face_count = 0;
    bool has_s = false, has_t = false;
    
    while (getline(file, line) and line != "end_header") {
        stringstream ss(line);
        string keyword;
        ss >> keyword;
        if (keyword == "element") {
            string type; ss >> type;
            if (type == "vertex") ss >> vertex_count;
            else if (type == "face") ss >> face_count;
        } else if (keyword == "property") {
            string type, name; ss >> type >> name;
            if (name == "s") has_s = true;
            if (name == "t") has_t = true;
        }
    }

    if (vertex_count == 0 or face_count == 0 or not has_s or not has_t) return false;

    vector<Vertex> vertices(vertex_count);
    for (long i = 0; i < vertex_count; ++i) {
        file >> vertices[i].pos.x_ >> vertices[i].pos.y_ >> vertices[i].pos.z_ >> vertices[i].tex.s >> vertices[i].tex.t;
    }
    
    triangles.clear();
    for (long i = 0; i < face_count; ++i) {
        int num_verts; file >> num_verts;
        if (num_verts == 3) {
            int i0, i1, i2; file >> i0 >> i1 >> i2;
            if (i0 < vertex_count and i1 < vertex_count and i2 < vertex_count) {
                triangles.push_back({vertices[i0], vertices[i1], vertices[i2]});
            }
        } else { getline(file, line); }
    }
    return true;
}

void rasterize_triangle_textured(vector<unsigned char>& buffer, size_t width, size_t height,
                                 const Vec3 p[3], const Triangle& original_tri, double cos_alpha, const Texture& tex) {
    int minX = max(0, (int)floor(min({p[0].x_, p[1].x_, p[2].x_})));
    int maxX = min((int)width - 1, (int)ceil(max({p[0].x_, p[1].x_, p[2].x_})));
    int minY = max(0, (int)floor(min({p[0].y_, p[1].y_, p[2].y_})));
    int maxY = min((int)height - 1, (int)ceil(max({p[0].y_, p[1].y_, p[2].y_})));

    double inv_z[3];
    Vec2 tex_over_z[3];
    for(int i=0; i<3; ++i) {
        inv_z[i] = 1.0 / original_tri.v[i].pos.z_;
        tex_over_z[i] = {original_tri.v[i].tex.s * inv_z[i], original_tri.v[i].tex.t * inv_z[i]};
    }

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vec3 P((double)x + 0.5, (double)y + 0.5, 0);
            
            double area = (p[1].y_ - p[2].y_) * (p[0].x_ - p[2].x_) + (p[2].x_ - p[1].x_) * (p[0].y_ - p[2].y_);
            if (abs(area) < 1e-9) continue;

            double w0 = ((p[1].y_ - p[2].y_) * (P.x_ - p[2].x_) + (p[2].x_ - p[1].x_) * (P.y_ - p[2].y_)) / area;
            double w1 = ((p[2].y_ - p[0].y_) * (P.x_ - p[2].x_) + (p[0].x_ - p[2].x_) * (P.y_ - p[2].y_)) / area;
            double w2 = 1.0 - w0 - w1;

            if (w0 >= 0 and w1 >= 0 and w2 >= 0) {
                double interp_inv_z = w0 * inv_z[0] + w1 * inv_z[1] + w2 * inv_z[2];
                double interp_s = (w0 * tex_over_z[0].s + w1 * tex_over_z[1].s + w2 * tex_over_z[2].s) / interp_inv_z;
                double interp_t = (w0 * tex_over_z[0].t + w1 * tex_over_z[1].t + w2 * tex_over_z[2].t) / interp_inv_z;

                int tex_x = static_cast<int>(interp_s * tex.width) % tex.width;
                int tex_y = static_cast<int>((1.0 - interp_t) * tex.height) % tex.height;
                if(tex_x < 0) tex_x += tex.width;
                if(tex_y < 0) tex_y += tex.height;

                int tex_idx = (tex_y * tex.width + tex_x) * tex.channels;
                
                double r = (double)tex.data[tex_idx + 0] * cos_alpha;
                double g = (double)tex.data[tex_idx + 1] * cos_alpha;
                double b = (double)tex.data[tex_idx + 2] * cos_alpha;
                
                size_t buffer_idx = (y * width + x) * 3;
                buffer[buffer_idx + 0] = static_cast<unsigned char>(r);
                buffer[buffer_idx + 1] = static_cast<unsigned char>(g);
                buffer[buffer_idx + 2] = static_cast<unsigned char>(b);
            }
        }
    }
}

void painter_algorithm_textures(
    string const& full_path_input_mesh,
    string const& full_path_input_texture,
    string const& full_path_output_image,
    double min_x_coordinate_in_projection_plane,
    double min_y_coordinate_in_projection_plane,
    double max_x_coordinate_in_projection_plane,
    double max_y_coordinate_in_projection_plane,
    size_t width_in_pixels,
    size_t height_in_pixels
) {
    Texture texture;
    if (not load_texture(full_path_input_texture, texture)) {
        cerr << "Error: No se pudo cargar el archivo de textura: " << full_path_input_texture << endl;
        return;
    }

    vector<Triangle> mesh_triangles;
    if (not load_ply_with_texture(full_path_input_mesh, mesh_triangles)) {
        cerr << "Error: No se pudo cargar el archivo de malla con texturas: " << full_path_input_mesh << endl;
        return;
    }

    vector<DrawableTriangle> drawable_triangles;
    for (const auto& tri : mesh_triangles) {
        DrawableTriangle dt;
        dt.tri = tri;
        dt.normal = (tri.v[2].pos - tri.v[0].pos).cross_product(tri.v[1].pos - tri.v[0].pos).get_unit_vec();
        dt.max_depth = max({tri.v[0].pos.norm(), tri.v[1].pos.norm(), tri.v[2].pos.norm()});
        drawable_triangles.push_back(dt);
    }
    
    sort(drawable_triangles.begin(), drawable_triangles.end());

    vector<unsigned char> image_buffer(width_in_pixels * height_in_pixels * 3, 0);
    Vec3 vision_vec_opposite(0, 0, -1);

    for (const auto& dt : drawable_triangles) {
        double cos_alpha = dt.normal.dot_product(vision_vec_opposite);
        if (cos_alpha <= 0) continue;

        Vec3 projected_points[3];
        bool triangle_is_valid = true;
        for (int i = 0; i < 3; ++i) {
            const auto& v = dt.tri.v[i].pos;
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
            rasterize_triangle_textured(image_buffer, width_in_pixels, height_in_pixels, projected_points, dt.tri, cos_alpha, texture);
        }
    }
    
    stbi_write_png(full_path_output_image.c_str(), width_in_pixels, height_in_pixels, 3, image_buffer.data(), width_in_pixels * 3);
}

int main() {
    try {
        cout << "Generando imagen con Painter's Algorithm y texturas..." << endl;
        painter_algorithm_textures(
            "esfera_textura.ply",
            "texture.png",
            "photo_of_sphere_textured.png",
            -2.0, -2.0, 2.0, 2.0,
            400, 400
        );
        cout << "Archivo 'photo_of_sphere_textured.png' generado." << endl;
    } catch (const exception& e) {
        cerr << "Ha ocurrido un error fatal: " << e.what() << endl;
        return 1;
    }
    return 0;
}