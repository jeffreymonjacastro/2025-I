#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//! EPS: Encapsulated PostScript
// Archivo en ASCII legible

using namespace std;

class EPSGenerator {
private:
  int xmin, ymin, xmax, ymax;
  std::string filename;

public:
  EPSGenerator(const std::string &fname, int xmin = -1, int ymin = -1,
               int xmax = 1, int ymax = 1)
      : filename(fname), xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {
    std::ofstream file(filename);

    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear el archivo " << filename
                << std::endl;
      return;
    }

    // Cabecera EPS
    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: " << xmin << " " << ymin << " " << xmax << " "
         << ymax << "\n";
    file << "%%Creator: Jamcy\n";
    file << "%%Title: Segmento\n";
    file << "%%EndComments\n\n";

    // Configurar color negro para el segmento
    file << "0 0 0 setrgbcolor\n";

    this->createEPSsegment(file);

    // Finalizar archivo
    file << "\nshowpage\n";
    file << "%%EOF\n";

    file.close();
  }

  void createEPSWithCenterDot(double dotRadius = 2.0) {
    std::ofstream file(filename);

    // Calcular centro
    double centerX = (xmax - xmin) / 2.0;
    double centerY = (ymax - ymin) / 2.0;

    // Crear el punto como un círculo relleno
    file << "newpath\n";
    file << centerX << " " << centerY << " " << dotRadius << " 0 360 arc\n";
    file << "fill\n";
  }

  void createEPSsegment(ofstream &file) {
    double centerX = (xmax + xmin) / 2.0;
    double centerY = (ymax + ymin) / 2.0;

    file << "newpath\n";
    file << centerX << " " << ymax << " moveto\n";
    file << xmax << " " << centerY << " lineto\n";
    file << centerX << " " << ymin << " lineto\n";
    file << xmin << " " << centerY << " lineto\n";
    file << centerX << " " << ymax << " lineto\n";
    file << "stroke\n";

    double smallCenterX = centerX / 2.0;
    double smallCenterY = centerY / 2.0;

    file << "newpath\n";
    file << smallCenterX << " " << ymax / 2.0 << " moveto\n";
    file << xmax / 2.0 << " " << smallCenterY << " lineto\n";
    file << smallCenterX << " " << ymin / 2.0 << " lineto\n";
    file << xmin / 2.0 << " " << smallCenterY << " lineto\n";
    file << smallCenterX << " " << ymax / 2.0 << " lineto\n";
    file << "stroke\n";
  }

  void f() {}
};

int main() {
  EPSGenerator generator("segment.eps", -400, -400, 400, 400);

  return 0;
}
