#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define pdd pair<double, double>

//! EPS: Encapsulated PostScript

using namespace std;

struct Rectangle {
  double x1, y1, x2, y2, x_center, y_center;
  Rectangle(double x1, double y1, double x2, double y2)
      : x1(x1), y1(y1), x2(x2), y2(y2) {
    x_center = (x1 + x2) / 2.0;
    y_center = (y1 + y2) / 2.0;
  }

  pdd UL() const {
    return {x1, y2};
  }

  pdd UR() const {
    return {x2, y2};
  }

  pdd BL() const {
    return {x1, y1};
  }

  pdd BR() const {
    return {x2, y1};
  }
};

double f(pdd point) {
  double x_center = 200.0;
  double y_center = 200.0;
  double radius = 100.0;
  return (point.first - x_center) * (point.first - x_center) + (point.second - y_center) * (point.second - y_center) - radius * radius;
}

class EPSGenerator {
private:
  int width, height;
  string filename;

  pdd getPoint(int point_index, Rectangle &rect) {
    pdd base_points[4] = {
        {rect.x1, rect.y_center}, // 0: lado izquierdo
        {rect.x_center, rect.y1}, // 1: lado inferior
        {rect.x2, rect.y_center}, // 2: lado derecho
        {rect.x_center, rect.y2}  // 3: lado superior
    };
    return base_points[point_index % 4];
  }

  void drawLine(ofstream &file, int from, int to, Rectangle &rect) {
    auto [x1, y1] = getPoint(from, rect);
    auto [x2, y2] = getPoint(to, rect);

    file << "newpath\n";
    file << x1 << " " << y1 << " moveto\n";
    file << x2 << " " << y2 << " lineto\n";
    file << "stroke\n";
  }

public:
  EPSGenerator(const string &fname, int w = 400, int h = 400)
      : filename(fname), width(w), height(h) {}

  void generate_eps(double div_h, double div_w) {
    ofstream file(filename);

    if (!file.is_open()) {
      cerr << "Error: No se pudo crear el archivo " << filename
           << endl;
      return;
    }

    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: 0 0 " << width << " " << height << "\n";
    file << "%%Creator: C++ EPS Generator\n";
    file << "%%Title: Marching Squares\n";
    file << "%%EndComments\n\n";

    file << "% Dibujar círculo continuo\n";
    file << "0 0 1 setrgbcolor\n";
    file << "1 setlinewidth\n";
    file << "newpath\n";
    file << "200 200 100 0 360 arc\n";
    file << "stroke\n\n";

    file << "% Dibujar líneas de marching squares\n";
    file << "1 0 0 setrgbcolor\n";
    file << "1 setlinewidth\n";

    draw_function(file, div_h, div_w);

    file << "\nshowpage\n";
    file << "%%EOF\n";

    file.close();
  }

  void draw_function(ofstream &file, double div_h, double div_w) {
    double delta_width = width / div_w;
    double delta_height = height / div_h;

    for (int i = 0; i < div_h; i++) {
      for (int j = 0; j < div_w; j++) {
        double x = j * delta_width;
        double y = i * delta_height;

        Rectangle rectangle(x, y, x + delta_width, y + delta_height);

        auto [x_min, y_min] = rectangle.BL();
        auto [x_max, y_max] = rectangle.UR();

        double x_center = rectangle.x_center;
        double y_center = rectangle.y_center;

        bool b0 = f(rectangle.BL()) > 0; // Esquina inferior izquierda
        bool b1 = f(rectangle.BR()) > 0; // Esquina inferior derecha
        bool b2 = f(rectangle.UR()) > 0; // Esquina superior derecha
        bool b3 = f(rectangle.UL()) > 0; // Esquina superior izquierda

        int which_case = b0 * 1 + b1 * 2 + b2 * 4 + b3 * 8;

        switch (which_case) {
        case 0:
        case 15:
          // Sin líneas
          break;

        case 1:
        case 14:
          drawLine(file, 0, 1, rectangle);
          break;

        case 2:
        case 13:
          drawLine(file, 1, 2, rectangle);
          break;

        case 3:
        case 12:
          drawLine(file, 0, 2, rectangle);
          break;

        case 4:
        case 11:
          drawLine(file, 2, 3, rectangle);
          break;

        case 5:
        case 10:
          drawLine(file, 0, 1, rectangle);
          drawLine(file, 2, 3, rectangle);
          break;

        case 6:
        case 9:
          drawLine(file, 1, 3, rectangle);
          break;

        case 7:
        case 8:
          drawLine(file, 0, 3, rectangle);
          break;
        }
      }
    }
    // Interpolación para mejorar
  }
};

int main() {
  EPSGenerator generator("Marching_Squares3.eps", 400, 400);

  generator.generate_eps(100, 100);

  cout << "\nListo! Puedes abrir el archivo con un visor PostScript" << endl;

  return 0;
}