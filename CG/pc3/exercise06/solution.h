#ifndef SOLUTION_H
#define SOLUTION_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

#define PI 3.14159265358979323846

class Punto3D {
private:
  double coord_x, coord_y, coord_z;

public:
  Punto3D() : coord_x(0.0), coord_y(0.0), coord_z(0.0) {}

  Punto3D(double x, double y, double z) : coord_x(x), coord_y(y), coord_z(z) {}

  double obtener_x() const { return coord_x; }
  double obtener_y() const { return coord_y; }
  double obtener_z() const { return coord_z; }

  void establecer_coordenadas(double x, double y, double z) {
    coord_x = x;
    coord_y = y;
    coord_z = z;
  }

  double producto_punto(const Punto3D &otro_punto) const {
    return coord_x * otro_punto.coord_x +
           coord_y * otro_punto.coord_y +
           coord_z * otro_punto.coord_z;
  }

  Punto3D producto_cruz(const Punto3D &otro_punto) const {
    double resultado_x = coord_y * otro_punto.coord_z - coord_z * otro_punto.coord_y;
    double resultado_y = coord_z * otro_punto.coord_x - coord_x * otro_punto.coord_z;
    double resultado_z = coord_x * otro_punto.coord_y - coord_y * otro_punto.coord_x;

    return Punto3D(resultado_x, resultado_y, resultado_z);
  }

  Punto3D operator+(const Punto3D &otro) const {
    return Punto3D(coord_x + otro.coord_x, coord_y + otro.coord_y, coord_z + otro.coord_z);
  }

  Punto3D operator-(const Punto3D &otro) const {
    return Punto3D(coord_x - otro.coord_x, coord_y - otro.coord_y, coord_z - otro.coord_z);
  }

  Punto3D operator*(double escalar) const {
    return Punto3D(coord_x * escalar, coord_y * escalar, coord_z * escalar);
  }

  Punto3D operator/(double escalar) const {
    if (escalar == 0.0) {
      throw runtime_error("Division por cero no permitida");
    }
    return Punto3D(coord_x / escalar, coord_y / escalar, coord_z / escalar);
  }

  double calcular_magnitud() const {
    return sqrt(producto_punto(*this));
  }

  Punto3D obtener_vector_unitario() const {
    double magnitud = calcular_magnitud();
    if (magnitud == 0.0) {
      throw runtime_error("No se puede normalizar un vector cero");
    }
    return *this / magnitud;
  }

  void mostrar_coordenadas() const {
    cout << "Coordenada X: " << coord_x << endl;
    cout << "Coordenada Y: " << coord_y << endl;
    cout << "Coordenada Z: " << coord_z << endl;
  }
};

double convertir_a_radianes(double angulo_grados) {
  return (PI * angulo_grados) / 180.0;
}

Punto3D rotar_punto_rodrigues(
    const Punto3D &punto_eje,
    const Punto3D &direccion_eje,
    const Punto3D &punto_a_rotar,
    double angulo_radianes) {
  Punto3D vector_rotacion = punto_a_rotar - punto_eje;

  Punto3D eje_unitario = direccion_eje.obtener_vector_unitario();

  double proyeccion_escalar = eje_unitario.producto_punto(vector_rotacion);
  Punto3D componente_paralela = eje_unitario * proyeccion_escalar;

  Punto3D componente_perpendicular = vector_rotacion - componente_paralela;

  Punto3D cruz_producto = eje_unitario.producto_cruz(componente_perpendicular);

  Punto3D vector_rotado = componente_paralela +
                          componente_perpendicular * cos(angulo_radianes) +
                          cruz_producto * sin(angulo_radianes);

  return punto_eje + vector_rotado;
}

int extraer_numero_desde_linea(const string &linea, const string &prefijo) {
  size_t tamaño_restante = linea.size() - prefijo.size();
  string numero_texto = linea.substr(prefijo.size(), tamaño_restante);
  return stoi(numero_texto);
}

void procesar_archivo_ply(
    const Punto3D &punto_eje,
    const Punto3D &direccion_eje,
    double angulo_radianes,
    const string &archivo_entrada,
    const string &archivo_salida) {
  ifstream entrada_ply(archivo_entrada);

  string linea_actual;
  string buscar_vertices = "element vertex ";
  string buscar_caras = "element face ";
  int cantidad_vertices = 0, cantidad_caras = 0;

  for (size_t contador = 0; contador < 20; contador++) {
    if (!getline(entrada_ply, linea_actual))
      break;

    if (linea_actual.find(buscar_vertices) != string::npos) {
      cantidad_vertices = extraer_numero_desde_linea(linea_actual, buscar_vertices);
    } else if (linea_actual.find(buscar_caras) != string::npos) {
      cantidad_caras = extraer_numero_desde_linea(linea_actual, buscar_caras);
    }
    if (linea_actual == "end_header")
      break;
  }

  ostringstream contenido_salida;
  contenido_salida << "ply\n"
                   << "format ascii 1.0\n"
                   << "element vertex " << cantidad_vertices << "\n"
                   << "property float x\n"
                   << "property float y\n"
                   << "property float z\n";

  contenido_salida << "element face " << cantidad_caras << "\n"
                   << "property list uchar uint vertex_indices\n"
                   << "end_header\n";

  double x_coord, y_coord, z_coord;
  for (int indice = 0; indice < cantidad_vertices; indice++) {
    entrada_ply >> x_coord >> y_coord >> z_coord;
    Punto3D vertice_original(x_coord, y_coord, z_coord);
    Punto3D vertice_rotado = rotar_punto_rodrigues(punto_eje, direccion_eje, vertice_original, angulo_radianes);

    contenido_salida << vertice_rotado.obtener_x() << " "
                     << vertice_rotado.obtener_y() << " "
                     << vertice_rotado.obtener_z() << "\n";
  }

  int num_vertices_cara;
  double v1, v2, v3;
  for (int indice = 0; indice < cantidad_caras; indice++) {
    entrada_ply >> num_vertices_cara >> v1 >> v2 >> v3;
    contenido_salida << num_vertices_cara << " " << v1 << " " << v2 << " " << v3 << "\n";
  }

  entrada_ply.close();

  ofstream salida_ply(archivo_salida);
  salida_ply << contenido_salida.str();
  salida_ply.close();
}

void rotate_mesh_around_line(
    const string &full_path_input_mesh,
    double px, double py, double pz,
    double dx, double dy, double dz,
    double alpha,
    const string &full_path_output_mesh) {
  Punto3D punto_eje(px, py, pz);
  Punto3D direccion_eje(dx, dy, dz);
  double angulo_radianes = convertir_a_radianes(alpha);

  procesar_archivo_ply(punto_eje, direccion_eje, angulo_radianes,
                       full_path_input_mesh, full_path_output_mesh);
}

#endif