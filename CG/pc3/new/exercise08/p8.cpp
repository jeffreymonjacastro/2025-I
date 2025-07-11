#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
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

struct Face {
    vector<int> vertices;
    Face(const vector<int>& verts) : vertices(verts) {}
};

struct Edge {
    int v1, v2;
    
    Edge(int a, int b) {
        v1 = min(a, b);
        v2 = max(a, b);
    }
    
    bool operator<(const Edge& other) const {
        if (v1 != other.v1) return v1 < other.v1;
        return v2 < other.v2;
    }
};

void catmull_clark(
    string input_path,
    int iterations,
    string output_path) {
    
    
    ifstream input_file(input_path);
    string line;
    
    while (getline(input_file, line)) {
        if (line.find("OFF") != string::npos || line.find("#") != string::npos || line.empty()) {
            continue;
        }
        break;
    }
    
    int num_vertices, num_faces, num_edges;
    istringstream iss(line);
    iss >> num_vertices >> num_faces >> num_edges;
    
    vector<Vector<ld>> vertices;
    vector<Face> faces;
    
    for (int i = 0; i < num_vertices; i++) {
        ld x, y, z;
        input_file >> x >> y >> z;
        vertices.push_back(Vector<ld>(x, y, z));
    }
    
    for (int i = 0; i < num_faces; i++) {
        int n;
        input_file >> n;
        vector<int> face_verts;
        for (int j = 0; j < n; j++) {
            int v;
            input_file >> v;
            face_verts.push_back(v);
        }
       
        string rest_of_line;
        getline(input_file, rest_of_line);
        
        faces.push_back(Face(face_verts));
    }
    
    input_file.close();
    
    for (int iter = 0; iter < iterations; iter++) {
        
       
        vector<Vector<ld>> face_points;
        for (const Face& face : faces) {
            Vector<ld> sum(0, 0, 0);
            for (int vi : face.vertices) {
                sum = sum + vertices[vi];
            }
            face_points.push_back(sum / (ld)face.vertices.size());
        }
        
        
        map<Edge, vector<int>> edge_to_faces;
        vector<set<int>> vertex_to_faces(vertices.size());
        vector<set<Edge>> vertex_to_edges(vertices.size());
        
        for (int fi = 0; fi < faces.size(); fi++) {
            const Face& face = faces[fi];
            int n = face.vertices.size();
            
            for (int vi : face.vertices) {
                vertex_to_faces[vi].insert(fi);
            }
            
            for (int i = 0; i < n; i++) {
                int v1 = face.vertices[i];
                int v2 = face.vertices[(i + 1) % n];
                Edge edge(v1, v2);
                edge_to_faces[edge].push_back(fi);
                vertex_to_edges[v1].insert(edge);
                vertex_to_edges[v2].insert(edge);
            }
        }
        
        
        map<Edge, Vector<ld>> edge_points;
        for (const auto& [edge, adj_faces] : edge_to_faces) {
            Vector<ld> v1_pos = vertices[edge.v1];
            Vector<ld> v2_pos = vertices[edge.v2];
            
            if (adj_faces.size() == 2) {
               
                Vector<ld> f1 = face_points[adj_faces[0]];
                Vector<ld> f2 = face_points[adj_faces[1]];
                edge_points[edge] = (f1 + f2 + v1_pos + v2_pos) / 4.0;
            } else {
               
                edge_points[edge] = (v1_pos + v2_pos) / 2.0;
            }
        }
        
       
        vector<Vector<ld>> new_vertices;
        
        for (int vi = 0; vi < vertices.size(); vi++) {
            const set<int>& adj_faces = vertex_to_faces[vi];
            const set<Edge>& adj_edges = vertex_to_edges[vi];
            
            if (adj_faces.empty()) {
                new_vertices.push_back(vertices[vi]);
                continue;
            }
            
          
            bool is_boundary = false;
            for (const Edge& edge : adj_edges) {
                if (edge_to_faces.at(edge).size() == 1) {
                    is_boundary = true;
                    break;
                }
            }
            
            if (is_boundary) {
             
                Vector<ld> sum = vertices[vi];
                int count = 1;
                
                for (const Edge& edge : adj_edges) {
                    if (edge_to_faces.at(edge).size() == 1) {
                        int neighbor = (edge.v1 == vi) ? edge.v2 : edge.v1;
                        sum = sum + vertices[neighbor];
                        count++;
                    }
                }
                
                new_vertices.push_back(sum / (ld)count);
            } else {
            
                int n = adj_faces.size();
              
                Vector<ld> F(0, 0, 0);
                for (int fi : adj_faces) {
                    F = F + face_points[fi];
                }
                F = F / (ld)n;
                
                
                Vector<ld> R(0, 0, 0);
                for (const Edge& edge : adj_edges) {
                    Vector<ld> edge_midpoint = (vertices[edge.v1] + vertices[edge.v2]) / 2.0;
                    R = R + edge_midpoint;
                }
                R = R / (ld)adj_edges.size();
                
               
                Vector<ld> P = vertices[vi];
                
          
                Vector<ld> new_pos = (F + R * 2.0 + P * (ld)(n - 3)) / (ld)n;
                new_vertices.push_back(new_pos);
            }
        }
        
   
        vector<Vector<ld>> all_vertices = new_vertices;
        vector<Face> new_faces;
        
 
        int face_point_start = all_vertices.size();
        for (const Vector<ld>& fp : face_points) {
            all_vertices.push_back(fp);
        }
        

        int edge_point_start = all_vertices.size();
        map<Edge, int> edge_to_idx;
        int edge_idx = 0;
        for (const auto& [edge, point] : edge_points) {
            all_vertices.push_back(point);
            edge_to_idx[edge] = edge_point_start + edge_idx;
            edge_idx++;
        }
        
      
        for (int fi = 0; fi < faces.size(); fi++) {
            const Face& original_face = faces[fi];
            int n = original_face.vertices.size();
            int face_point_idx = face_point_start + fi;
            
            for (int i = 0; i < n; i++) {
                int curr_vert = original_face.vertices[i];
                int next_vert = original_face.vertices[(i + 1) % n];
                int prev_vert = original_face.vertices[(i + n - 1) % n];
                
                Edge edge_to_next(curr_vert, next_vert);
                Edge edge_from_prev(prev_vert, curr_vert);
                
                int edge_to_next_idx = edge_to_idx[edge_to_next];
                int edge_from_prev_idx = edge_to_idx[edge_from_prev];
                
            
                vector<int> quad = {
                    curr_vert,
                    edge_to_next_idx,
                    face_point_idx,
                    edge_from_prev_idx
                };
                
                new_faces.push_back(Face(quad));
            }
        }
        
        vertices = all_vertices;
        faces = new_faces;
    }
    

    ostringstream content;
    content << "OFF\n";
    content << vertices.size() << " " << faces.size() << " 0\n";
    
    for (const Vector<ld>& v : vertices) {
        content << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
    }
    
    for (const Face& f : faces) {
        content << f.vertices.size();
        for (int v : f.vertices) {
            content << " " << v;
        }
        content << "\n";
    }
    
    ofstream output_file(output_path);
    output_file << content.str();
    output_file.close();
}

int main() {
    catmull_clark(
        "cube.off",
        3,  
        "cube_3_iter.off"
    );
    
    return 0;
}