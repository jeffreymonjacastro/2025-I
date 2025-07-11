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

void loop(
    string full_path_input_mesh,
    int number_of_iterations,
    string full_path_output_mesh) {
    

    ifstream input_file(full_path_input_mesh);
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
    

    for (int iter = 0; iter < number_of_iterations; iter++) {
        
    
        map<Edge, vector<int>> edge_to_faces;
        vector<set<int>> vertex_to_faces(vertices.size());
        vector<set<int>> vertex_neighbors(vertices.size());
        
        for (int fi = 0; fi < faces.size(); fi++) {
            const Face& face = faces[fi];
            
     
            if (face.vertices.size() != 3) {
                cout << "Error: Loop subdivision requiere mallas triangulares" << endl;
                return;
            }
            
         
            for (int vi : face.vertices) {
                vertex_to_faces[vi].insert(fi);
            }
            
       
            for (int i = 0; i < 3; i++) {
                int v1 = face.vertices[i];
                int v2 = face.vertices[(i + 1) % 3];
                
                Edge edge(v1, v2);
                edge_to_faces[edge].push_back(fi);
                
                vertex_neighbors[v1].insert(v2);
                vertex_neighbors[v2].insert(v1);
            }
        }
        
     
        vector<Vector<ld>> new_vertices;
        
        for (int vi = 0; vi < vertices.size(); vi++) {
            const set<int>& neighbors = vertex_neighbors[vi];
            int n = neighbors.size(); 
            

            bool is_boundary = false;
            for (const auto& [edge, adj_faces] : edge_to_faces) {
                if ((edge.v1 == vi || edge.v2 == vi) && adj_faces.size() == 1) {
                    is_boundary = true;
                    break;
                }
            }
            
            if (is_boundary) {
         
                Vector<ld> boundary_neighbors(0, 0, 0);
                int boundary_count = 0;
                
                for (const auto& [edge, adj_faces] : edge_to_faces) {
                    if (adj_faces.size() == 1) {
                        if (edge.v1 == vi) {
                            boundary_neighbors = boundary_neighbors + vertices[edge.v2];
                            boundary_count++;
                        } else if (edge.v2 == vi) {
                            boundary_neighbors = boundary_neighbors + vertices[edge.v1];
                            boundary_count++;
                        }
                    }
                }
                
                if (boundary_count == 2) {
              
                    new_vertices.push_back(vertices[vi] * 0.75 + boundary_neighbors * 0.125);
                } else {
                   
                    new_vertices.push_back(vertices[vi]);
                }
            } else {
                ld beta;
                if (n == 3) {
                    beta = 3.0/16.0;
                } else if (n > 3) {
                    beta = 3.0 / (8.0 * n);
                } else {
                 
                    new_vertices.push_back(vertices[vi]);
                    continue;
                }
                
           
                Vector<ld> neighbor_sum(0, 0, 0);
                for (int neighbor : neighbors) {
                    neighbor_sum = neighbor_sum + vertices[neighbor];
                }
                
          
                Vector<ld> new_pos = vertices[vi] * (1.0 - n * beta) + neighbor_sum * beta;
                new_vertices.push_back(new_pos);
            }
        }
        
       
        map<Edge, Vector<ld>> edge_vertices;
        
        for (const auto& [edge, adj_faces] : edge_to_faces) {
            Vector<ld> v1_pos = vertices[edge.v1];
            Vector<ld> v2_pos = vertices[edge.v2];
            
            if (adj_faces.size() == 2) {
            
                int f1 = adj_faces[0];
                int f2 = adj_faces[1];
                
              
                int v3 = -1, v4 = -1;
                for (int v : faces[f1].vertices) {
                    if (v != edge.v1 && v != edge.v2) {
                        v3 = v;
                        break;
                    }
                }
                for (int v : faces[f2].vertices) {
                    if (v != edge.v1 && v != edge.v2) {
                        v4 = v;
                        break;
                    }
                }
                
            
                Vector<ld> edge_pos = (v1_pos + v2_pos) * (3.0/8.0) + (vertices[v3] + vertices[v4]) * (1.0/8.0);
                edge_vertices[edge] = edge_pos;
            } else {
               
                edge_vertices[edge] = (v1_pos + v2_pos) * 0.5;
            }
        }
        
    
        vector<Vector<ld>> all_vertices = new_vertices;
        vector<Face> new_faces;
        
    
        map<Edge, int> edge_to_index;
        int edge_vertex_start = all_vertices.size();
        int edge_idx = 0;
        
        for (const auto& [edge, vertex] : edge_vertices) {
            all_vertices.push_back(vertex);
            edge_to_index[edge] = edge_vertex_start + edge_idx;
            edge_idx++;
        }
        
     
        for (const Face& original_face : faces) {
            int v0 = original_face.vertices[0];
            int v1 = original_face.vertices[1];
            int v2 = original_face.vertices[2];
            
 
            Edge e01(v0, v1);
            Edge e12(v1, v2);
            Edge e20(v2, v0);
            
            int e01_idx = edge_to_index[e01];
            int e12_idx = edge_to_index[e12];
            int e20_idx = edge_to_index[e20];
            
           
            new_faces.push_back(Face({v0, e01_idx, e20_idx}));     
            new_faces.push_back(Face({v1, e12_idx, e01_idx}));   
            new_faces.push_back(Face({v2, e20_idx, e12_idx}));    
            new_faces.push_back(Face({e01_idx, e12_idx, e20_idx})); 
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
    
    ofstream output_file(full_path_output_mesh);
    output_file << content.str();
    output_file.close();
}

int main() {
    loop(
        "cube_t.off",
        3,
        "cube_t_iter_3.off"
    );
    
    return 0;
}