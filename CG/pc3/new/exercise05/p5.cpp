#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

pair<double, double> getTextureCoords(point center, double x, double y, double z) {
  double dx = x - center.x;
  double dy = y - center.y;
  double dz = z - center.z;
  
  double r = sqrt(dx*dx + dy*dy + dz*dz);
  
  double theta = acos(-dz / r);
  
  double phi = atan2(dy, dx);
  if (phi < 0) phi += 2*M_PI;
  
  double s = phi / (2*M_PI);
  double t = theta / M_PI;
  
  return {s, t};
}

int extract_target(string line, string target) {
   int size = line.size() - target.size();
   target = line.substr(target.size(), size);
   return stoi(target); 
}

void sphere_with_texture(string full_path_input_ply,
                       string full_path_texture,
                       point center,
                       string full_path_output_ply) {
   
   ifstream ply_input_file(full_path_input_ply);
   
   string line;
   string look_for_ev = "element vertex ";
   string look_for_ef = "element face ";
   int element_vertex, element_face;
   
   for (size_t i = 0; i < 9; i++) {
       getline(ply_input_file, line);
       
       if (line.find(look_for_ev) != string::npos) {
           element_vertex = extract_target(line, look_for_ev);
       } else if (line.find(look_for_ef) != string::npos) {
           element_face = extract_target(line, look_for_ef);
       }
   }
   
   ostringstream content;
   
   content << "ply\n"
           << "format ascii 1.0\n"
           << "element vertex " << element_vertex << "\n"
           << "property float x\n"
           << "property float y\n"
           << "property float z\n"
           << "property float s\n"
           << "property float t\n"
           << "element face " << element_face << "\n"
           << "property list uchar uint vertex_indices\n"
           << "end_header\n";
   
   content.str().reserve(element_vertex * 80 + element_face * 20);
   
   double x, y, z;
   for (int i = 0; i < element_vertex; i++) {
       ply_input_file >> x >> y >> z;
       auto st = getTextureCoords(center, x, y, z);
       content << x << " " << y << " " << z << " " << st.first << " " << st.second << "\n";
   }
   
   int num;
   double v1, v2, v3;
   for (int i = 0; i < element_face; i++) {
       ply_input_file >> num >> v1 >> v2 >> v3;
       content << num << " " << v1 << " " << v2 << " " << v3 << "\n";
   }
   
   ply_input_file.close();
   
   ofstream ply_output_file(full_path_output_ply);
   ply_output_file << content.str();
   ply_output_file.close();
   
   cout << "Textura aplicada con éxito: " << full_path_output_ply << endl;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);
   
   sphere_with_texture("esfera.ply", "algo", point(0,0,2), "esfera_textura.ply");
}