#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Punto3D {
private:
  double coord_x, coord_y, coord_z;

public:
  Punto3D() : coord_x(0.0), coord_y(0.0), coord_z(0.0) {}
  Punto3D(double x, double y, double z) : coord_x(x), coord_y(y), coord_z(z) {}

  double obtener_x() const { return coord_x; }
  double obtener_y() const { return coord_y; }
  double obtener_z() const { return coord_z; }

  Punto3D operator+(const Punto3D &otro) const {
    return Punto3D(coord_x + otro.coord_x, coord_y + otro.coord_y, coord_z + otro.coord_z);
  }
};

void translate_mesh(string full_path_input_mesh, double dx, double dy, double dz, string full_path_output_mesh) {
  ifstream archivo_entrada(full_path_input_mesh);
  ofstream archivo_salida(full_path_output_mesh);

  string linea;
  int num_vertices = 0, num_caras = 0;
  bool leyendo_vertices = false;
  bool leyendo_caras = false;
  int contador_vertices = 0;
  int contador_caras = 0;

  Punto3D vector_traslacion(dx, dy, dz);

  while (getline(archivo_entrada, linea)) {
    if (linea.find("element vertex") != string::npos) {
      istringstream ss(linea);
      string palabra1, palabra2;
      ss >> palabra1 >> palabra2 >> num_vertices;
      archivo_salida << linea << "\n";
    } else if (linea.find("element face") != string::npos) {
      istringstream ss(linea);
      string palabra1, palabra2;
      ss >> palabra1 >> palabra2 >> num_caras;
      archivo_salida << linea << "\n";
    } else if (linea == "end_header") {
      archivo_salida << linea << "\n";
      leyendo_vertices = true;
    } else if (leyendo_vertices && contador_vertices < num_vertices) {
      istringstream ss(linea);
      double x, y, z;
      ss >> x >> y >> z;

      Punto3D punto_original(x, y, z);
      Punto3D punto_trasladado = punto_original + vector_traslacion;

      archivo_salida << punto_trasladado.obtener_x() << " "
                     << punto_trasladado.obtener_y() << " "
                     << punto_trasladado.obtener_z();

      string resto;
      if (getline(ss, resto) && !resto.empty()) {
        archivo_salida << resto;
      }
      archivo_salida << "\n";

      contador_vertices++;
      if (contador_vertices == num_vertices) {
        leyendo_vertices = false;
        leyendo_caras = true;
      }
    } else if (leyendo_caras && contador_caras < num_caras) {
      archivo_salida << linea << "\n";
      contador_caras++;
    } else {
      archivo_salida << linea << "\n";
    }
  }

  archivo_entrada.close();
  archivo_salida.close();
}
