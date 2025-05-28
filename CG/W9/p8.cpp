#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//! EPS: Encapsulated PostScript
// Archivo en ASCII legible

using namespace std;

double f(double x, double y) {
  return x * x + y * y - 100 * 100;
}

class EPSGenerator {
private:
  int width, height;
  std::string filename;

public:
  EPSGenerator(const std::string &fname, int w = 400, int h = 400)
      : filename(fname), width(w), height(h) {}

  void draw_function(double div_h, double div_w) {

    std::ofstream file(filename);

    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear el archivo " << filename
                << std::endl;
      return;
    }

    double delta_width = width / div_w;
    double delta_height = height / div_h;

    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: 0 0 " << width << " " << height << "\n";
    file << "%%Creator: C++ EPS Generator\n";
    file << "%%Title: Linea\n";
    file << "%%EndComments\n\n";
    file << "0 0 0 setrgbcolor\n";

    file << "newpath\n";
    file << "0 0 moveto\n";

    for (int j = 0; j < div_w; j++) {
      for (int i = 0; i < div_h; i++) {
        double x = i * delta_height;
        double y = j * delta_width;

        double x_min = x + delta_height;
        double y_min = y;
        double x_max = x;
        double y_max = y + delta_width;

        double center_x = (x_min + x_max) / 2.0;
        double center_y = (y_min + y_max) / 2.0;

        bool is_positive_3 = int(f(x, y) > 0);
        bool is_positive_2 = int(f(x + delta_height, y) > 0);
        bool is_positive_1 = int(f(x + delta_height, y + delta_width) > 0);
        bool is_positive_0 = int(f(x, y + delta_width) > 0);

        int which_case = is_positive_0 * 1 + is_positive_1 * 2 + is_positive_2 * 4 + is_positive_3 * 8;

        switch (which_case) {
        case 0:

          break;

        case 1:

          file << center_x << " " << y_min << " lineto\n";
          file << x_min << " " << center_y << " lineto\n";

          break;

        case 2:

          file << x_min << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 3:
          file << center_x << " " << y_min << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";
          break;

        case 4:
          file << x_max << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 5:
          file << center_x << " " << y_min << " lineto\n";
          file << x_max << " " << center_y << " lineto\n";

          file << x_min << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 6:
          file << x_min << " " << center_y << " lineto\n";
          file << x_max << " " << center_y << " lineto\n";

          break;

        case 7:
          file << center_x << " " << y_min << " lineto\n";
          file << x_max << " " << center_y << " lineto\n";

          break;

        case 8:
          file << center_x << " " << y_min << " lineto\n";
          file << x_max << " " << center_y << " lineto\n";

          break;

        case 9:
          file << x_min << " " << center_y << " lineto\n";
          file << x_max << " " << center_y << " lineto\n";

          break;

        case 10:
          file << center_x << " " << y_min << " lineto\n";
          file << x_min << " " << center_y << " lineto\n";

          file << x_max << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 11:
          file << x_max << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 12:
          file << center_x << " " << y_min << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 13:
          file << x_min << " " << center_y << " lineto\n";
          file << center_x << " " << y_max << " lineto\n";

          break;

        case 14:
          file << center_x << " " << y_min << " lineto\n";
          file << x_min << " " << center_y << " lineto\n";

          break;

        case 15:

          break;
        }
      }
    }

    // Finalizar archivo
    file << "\nshowpage\n";
    file << "%%EOF\n";

    file.close();
  }
};

int main() {
  // Crear generador EPS con dimensiones personalizables
  EPSGenerator generator("Marching_Squares.eps", 400, 400);

  // Generar archivo con punto de radio 3.0
  generator.draw_function(40, 40);

  std::cout << "\n¡Listo! Puedes abrir el archivo con un visor PostScript o "
               "convertirlo a PDF/PNG."
            << std::endl;

  return 0;
}
