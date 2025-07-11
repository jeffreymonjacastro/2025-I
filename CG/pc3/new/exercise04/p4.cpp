#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

struct point {
    double x, y, z;
    
    point(double xa, double ya, double za) : x(xa), y(ya), z(za) {}
    
    void print() {
        cout << "(" << x << ", " << y << ", " << z << ") " << "\t";
    }
};

double to_radian(double angle) {
    return (M_PI * angle) / 180.0;
}

void writePLY(const vector<vector<point>>& sphere, const string& filename) {
  
    int num_points = sphere.size() * sphere[0].size();
    int phi_360 = sphere[0].size();
    int theta_181 = sphere.size();
    
    int triangular_faces_top = phi_360;    
    int triangular_faces_bottom = phi_360; 
    int tri_faces = 2*phi_360 * (theta_181 - 3);
    int num_faces = triangular_faces_top + triangular_faces_bottom + tri_faces;


    ostringstream content;
    

    content << "ply\n"
            << "format ascii 1.0\n"
            << "element vertex " << num_points << "\n"
            << "property float x\n"
            << "property float y\n"
            << "property float z\n"
            << "element face " << num_faces << "\n"
            << "property list uchar int vertex_indices\n"
            << "end_header\n";

 
    content.str().reserve(num_points * 50 + num_faces * 20);
    
    for (size_t i = 0; i < sphere.size(); i++) {
        for (size_t j = 0; j < sphere[0].size(); j++) {
            const point& p = sphere[i][j];
            content << p.x << " " << p.y << " " << p.z << "\n";
        }
    }
    
 
    int faces_written = 0;
    for (int i = 0; i < theta_181 - 1; i++) {
        for (int j = 0; j < phi_360; j++) {
            int current_ring_start = i * phi_360;
            int next_ring_start = (i + 1) * phi_360;
            
            int p1 = current_ring_start + j;
            int p2 = current_ring_start + (j + 1) % phi_360;
            int p3 = next_ring_start + j;
            int p4 = next_ring_start + (j + 1) % phi_360;
            
            if (i == 0) {
                content << "3 " << p1 << " " << p4 << " " << p3 << "\n";
                faces_written++;
            }
            else if (i == theta_181 - 2) {
                content << "3 " << p1 << " " << p2 << " " << p3 << "\n";
                faces_written++;
            }
            else {
                content << "3 " << p1 << " " << p2 << " " << p3 << "\n"
                        << "3 " << p2 << " " << p4 << " " << p3 << "\n";
                faces_written += 2;
            }
        }
    }
    
   
    ofstream ply_file(filename.c_str());
    ply_file << content.str();
    ply_file.close();
    
    cout << "Puntos: " << num_points << endl;
    cout << "Caras: " << faces_written << endl;
    cout << "Archivo PLY generado con éxito: " << filename << endl;
}

void sphere_with_quadrilateral_faces(string full_path_output_file, double radius, 
                                  double center_x, double center_y, double center_z) {
   double delta_theta = 1.0;
   double delta_phi = 1.0;
   
 
   int expected_rings = (int)(180.0 / delta_theta) + 1;
   int expected_points_per_ring = (int)(360.0 / delta_phi) + 1;
   
   vector<vector<point>> sphere;
   sphere.reserve(expected_rings);
   
   for (double theta = 0; theta <= 180; theta += delta_theta) {
       vector<point> ring;
       ring.reserve(expected_points_per_ring);
       
       double currentR = radius * sin(to_radian(theta));
       double z = -radius * cos(to_radian(theta)) + center_z;
       
       if (theta == 0 or theta == 180) {
           double x = center_x;
           double y = center_y;
   
           for (int angle = 0; angle <= 360; angle++) {
               ring.push_back(point(x, y, z));
           }
       } else {
           for (double angle = 0; angle <= 360.0; angle += delta_phi) {
               double x = currentR * cos(to_radian(angle)) + center_x;
               double y = currentR * sin(to_radian(angle)) + center_y;
               ring.push_back(point(x, y, z));
           }
       }
       
       sphere.push_back(move(ring)); 
   }
   
   cout << "Anillos: " << sphere.size() << endl;
   cout << "Puntos por anillo: " << sphere[0].size() << endl;
   
   writePLY(sphere, full_path_output_file);
}

int main() {
   
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    sphere_with_quadrilateral_faces("esfera.ply", 1, 0, 0, 2);
    return 0;
}