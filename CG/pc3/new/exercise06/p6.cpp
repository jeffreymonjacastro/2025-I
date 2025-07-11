#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#define ll long long
#define ld long double

using namespace std;

template <typename T>
int sgn(T val) {
   if (val < static_cast<T>(0)) return -1;
   if (val > static_cast<T>(0)) return 1;
   return 0;
}

template <typename T>
class Vector {
private:
   bool is_int;

public:
   T x_, y_, z_;

   Vector() : x_(T{}), y_(T{}), z_(T{}) {
       is_int = is_same<int, T>::value;
   }

   Vector(int x, int y, int z) {
       is_int = is_same<int, T>::value;
       if (is_int) {
           x_ = static_cast<T>(x);
           y_ = static_cast<T>(y);
           z_ = static_cast<T>(z);
       } else {
           x_ = static_cast<T>(x);
           y_ = static_cast<T>(y);
           z_ = static_cast<T>(z);
       }
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

   Vector(long double x, long double y, long double z) {
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

   Vector(long long x, long long y, long long z) {
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
       ld x = (ld)x_/escalar;
       ld y = (ld)y_/escalar;
       ld z = (ld)z_/escalar;

       return Vector<ld>(x,y,z);
   }

   Vector<ld> operator*(const ld escalar)const {
       ld x = (ld)x_*escalar;
       ld y = (ld)y_*escalar;
       ld z = (ld)z_*escalar;

       return Vector<ld>(x,y,z);
   }

   Vector<ld> operator+(const Vector<T>& other) const {
       if (is_int) {
           ll x = (ll)x_ + (ll)other.x_;
           ll y = (ll)y_ + (ll)other.y_;
           ll z = (ll)z_ + (ll)other.z_;
           return Vector<ld>((ld)x, (ld)y, (ld)z);
       } else {
           ld x = (ld)x_ + (ld)other.x_;
           ld y = (ld)y_ + (ld)other.y_;
           ld z = (ld)z_ + (ld)other.z_;
           return Vector<ld>(x, y, z);
       }
   }

   Vector<ld> operator-(const Vector<T>& other) const {
       if (is_int) {
           ll x = (ll)x_ - (ll)other.x_;
           ll y = (ll)y_ - (ll)other.y_;
           ll z = (ll)z_ - (ll)other.z_;
           return Vector<ld>((ld)x, (ld)y, (ld)z);
       } else {
           ld x = (ld)x_ - (ld)other.x_;
           ld y = (ld)y_ - (ld)other.y_;
           ld z = (ld)z_ - (ld)other.z_;
           return Vector<ld>(x, y, z);
       }
   }

   bool operator==(const Vector<T>& other) const {
       return x_ == other.x_ and y_ == other.y_ and z_ == other.z_;
   }

   ld norm() {
       return sqrt(dot_product(*this));
   }

   Vector<ld> get_unit_vec(){
       ld magnitude = this->norm();
       if(magnitude == 0) {
           throw std::runtime_error("Cannot normalize zero vector");
       }
       return *this / magnitude;
   }

   void print() {
       cout << "X :" << x_ << endl;
       cout << "Y :" << y_ << endl;
       cout << "Z :" << z_ << endl;
   }
};

bool my_xor(bool a, bool b) {
   return a != b;
}


double to_radian(double angle) {
    return (M_PI * angle) / 180.0;
}

Vector<ld> rotate_point_rodrigues(Vector<ld> point, Vector<ld> director, Vector<ld> to_rotate, double angle){
   auto Rotating_vector = to_rotate - point;
   auto director_unit = director.get_unit_vec();
   ld dot_axis_rotating = director_unit.dot_product(Rotating_vector);
   auto R_parallel = director_unit * dot_axis_rotating;
   auto R_orthogonal = Rotating_vector - R_parallel;
   auto cross_product = director_unit.cross_product(R_orthogonal);
   auto rotated_vector = R_parallel + 
                        R_orthogonal * cos(angle) + 
                        cross_product * sin(angle);
   return point + rotated_vector;
}

int extract_target(string line, string target) {
   int size = line.size() - target.size();
   target = line.substr(target.size(), size);
   return stoi(target); 
}

void write_off(Vector<ld> point, Vector<ld> director, double angle, string full_path_input_mesh, string full_path_output_mesh ){
    ifstream input_file(full_path_input_mesh);
    
    string line;
    while (getline(input_file, line)) {
        if (line.find("OFF") != string::npos or line.find("#") != string::npos or line.empty()) {
            continue;
        }
        break;
    }
    
    int vertices, faces, edges;
    istringstream iss(line);
    iss >> vertices >> faces >> edges;
    
    ostringstream content;
    content << "OFF\n" << vertices << " " << faces << " " << edges << "\n";
    
    double x, y, z;
    for (int i = 0; i < vertices; i++) {
        input_file >> x >> y >> z;
        Vector<ld> original_point(x, y, z);
        auto rotated = rotate_point_rodrigues(point, director, original_point, angle);
        content << rotated.x_ << " " << rotated.y_ << " " << rotated.z_ << "\n";
    }
    
    int num_vertices_face;
    string rest_of_line;
    for (int i = 0; i < faces; i++) {
        input_file >> num_vertices_face;
        getline(input_file, rest_of_line);
        content << num_vertices_face << rest_of_line << "\n";
    }
    
    input_file.close();
    
    ofstream output_file(full_path_output_mesh);
    output_file << content.str();
    output_file.close();
}
void write_ply(Vector<ld> point, Vector<ld> director, double angle, string full_path_input_mesh, string full_path_output_mesh ){
    ifstream ply_input_file(full_path_input_mesh);
    
    string line;
    string look_for_ev = "element vertex ";
    string look_for_ef = "element face ";
    int element_vertex, element_face;
    bool has_texture = false;
    
    for (size_t i = 0; i < 20; i++) {
        if (!getline(ply_input_file, line)) break;
        
        if (line.find(look_for_ev) != string::npos) {
            element_vertex = extract_target(line, look_for_ev);
        } else if (line.find(look_for_ef) != string::npos) {
            element_face = extract_target(line, look_for_ef);
        }
        
        if (line.find("property float s") != string::npos or line.find("property float t") != string::npos) {
            has_texture = true;
        }
        
        if (line == "end_header") break;
    }
    
    ostringstream content;
    
    content << "ply\n"
            << "format ascii 1.0\n"
            << "element vertex " << element_vertex << "\n"
            << "property float x\n"
            << "property float y\n"
            << "property float z\n";
    
    if (has_texture) {
        content << "property float s\n"
                << "property float t\n";
    }
    
    content << "element face " << element_face << "\n"
            << "property list uchar uint vertex_indices\n"
            << "end_header\n";
    
    if (has_texture) {
        double x, y, z, s, t;
        for (int i = 0; i < element_vertex; i++) {
            ply_input_file >> x >> y >> z >> s >> t;
            Vector<ld> original_point(x, y, z);
            auto rotated = rotate_point_rodrigues(point, director, original_point, angle);
            content << rotated.x_ << " " << rotated.y_ << " " << rotated.z_ << " " << s << " " << t << "\n";
        }
    } else {
        double x, y, z;
        for (int i = 0; i < element_vertex; i++) {
            ply_input_file >> x >> y >> z;
            Vector<ld> original_point(x, y, z);
            auto rotated = rotate_point_rodrigues(point, director, original_point, angle);
            content << rotated.x_ << " " << rotated.y_ << " " << rotated.z_ << "\n";
        }
    }
    
    int num;
    double v1, v2, v3;
    for (int i = 0; i < element_face; i++) {
        ply_input_file >> num >> v1 >> v2 >> v3;
        content << num << " " << v1 << " " << v2 << " " << v3 << "\n";
    }
    
    ply_input_file.close();
    
    ofstream ply_output_file(full_path_output_mesh);
    ply_output_file << content.str();
    ply_output_file.close();
}


void rotate_mesh_around_line(
string full_path_input_mesh,
double px, double py , double pz,
double dx , double dy, double dz ,
double alpha,
string full_path_output_mesh){

   ifstream open_file(full_path_input_mesh);
   string type_of_file;
   getline(open_file, type_of_file);

   bool is_ply = type_of_file.find("ply") != string::npos;
   bool is_off = type_of_file.find("OFF") != string::npos or type_of_file.find("off") != string::npos;

   Vector<ld> point(px, py, pz);
   Vector<ld> director(dx, dy, dz);
   double angle_rad = to_radian(alpha);

   open_file.close();

   if(is_ply){
       write_ply(point, director, angle_rad, full_path_input_mesh, full_path_output_mesh);    
   }

   if(is_off){
       write_off(point, director, angle_rad, full_path_input_mesh, full_path_output_mesh);
   }
}


int main(){
    rotate_mesh_around_line("cube.off",0,0,0,0,0,1,45,"cube_rotated.off");
}