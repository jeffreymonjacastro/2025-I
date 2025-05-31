#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

class QTNode {
private:
  double xmin, ymin, xmax, ymax;
  char val;
  bool is_leaf;
  function<double(double, double)> f;
  QTNode *NE;
  QTNode *NO;
  QTNode *SE;
  QTNode *SO;

public:
  QTNode(double xmin, double ymin, double xmax, double ymax, function<double(double, double)> func)
      : NE(nullptr), NO(nullptr), SO(nullptr), SE(nullptr),
        is_leaf(true),
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax),
        val(' '), f(func) {}

  ~QTNode() {
    delete NE;
    delete NO;
    delete SE;
    delete SO;
  }

  void draw_line(fstream &file, double x1, double y1, double x2, double y2) {
    file << "newpath" << endl;
    file << x1 << " " << y1 << " moveto" << endl;
    file << x2 << " " << y2 << " lineto" << endl;
    file << "stroke" << endl;
  }

  void draw_cell(fstream &file) {
    file << "gsave" << endl;

    file << "0.5 0.5 0.5 setrgbcolor" << endl;
    file << "0.2 setlinewidth" << endl;

    file << "newpath" << endl;
    file << xmin << " " << ymin << " moveto" << endl;
    file << xmax << " " << ymin << " lineto" << endl;
    file << xmax << " " << ymax << " lineto" << endl;
    file << xmin << " " << ymax << " lineto" << endl;
    file << "closepath" << endl;
    file << "stroke" << endl;

    file << "grestore" << endl;
    file << "1 0 0 setrgbcolor" << endl;
    file << "0.5 setlinewidth" << endl;
  }

  double intersection(double x1, double y1, double x2, double y2, bool is_horizontal = true) {
    double f1 = f(x1, y1);
    double f2 = f(x2, y2);

    if (f1 * f2 > 0) {
      return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
    }

    for (int i = 0; i < 20; i++) {
      double xcenter = (x1 + x2) / 2;
      double ycenter = (y1 + y2) / 2;
      double f_mid = f(xcenter, ycenter);

      if (abs(f_mid) < 1e-6)
        break;

      if ((f1 < 0 && f_mid < 0) || (f1 > 0 && f_mid > 0)) {
        x1 = xcenter;
        y1 = ycenter;
        f1 = f_mid;
      } else {
        x2 = xcenter;
        y2 = ycenter;
        f2 = f_mid;
      }
    }

    return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
  }

  bool should_split() {
    int samples = 50;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis_x(xmin, xmax);
    uniform_real_distribution<double> dis_y(ymin, ymax);

    int pos = 0;
    int neg = 0;

    if (f(xmin, ymin) > 0)
      pos++;
    else
      neg++;
    if (f(xmax, ymin) > 0)
      pos++;
    else
      neg++;
    if (f(xmax, ymax) > 0)
      pos++;
    else
      neg++;
    if (f(xmin, ymax) > 0)
      pos++;
    else
      neg++;

    if (pos == 0 || neg == 0) {
      for (int i = 0; i < samples; i++) {
        double x = dis_x(gen);
        double y = dis_y(gen);
        double value = f(x, y);

        if (value > 0)
          pos++;
        else if (value < 0)
          neg++;

        if (pos > 0 && neg > 0)
          break;
      }
    }

    return (pos > 0 && neg > 0);
  }

  void matching_squares(fstream &file) {
    double xcenter = (xmin + xmax) / 2;
    double ycenter = (ymin + ymax) / 2;

    int b0 = int(f(xmin, ymin) > 0);
    int b1 = int(f(xmax, ymin) > 0);
    int b2 = int(f(xmax, ymax) > 0);
    int b3 = int(f(xmin, ymax) > 0);

    int binary = b0 + 2 * b1 + 4 * b2 + 8 * b3;

    double left = intersection(xmin, ymin, xmin, ymax, false);
    double right = intersection(xmax, ymin, xmax, ymax, false);
    double bottom = intersection(xmin, ymin, xmax, ymin, true);
    double top = intersection(xmin, ymax, xmax, ymax, true);

    switch (binary) {
    case 0:
    case 15:
      break;
    case 1:
      draw_line(file, xmin, left, bottom, ymin);
      break;
    case 2:
      draw_line(file, bottom, ymin, xmax, right);
      break;
    case 3:
      draw_line(file, xmin, left, xmax, right);
      break;
    case 4:
      draw_line(file, top, ymax, xmax, right);
      break;
    case 5:
      if (f(xcenter, ycenter) > 0) {
        draw_line(file, xmin, left, bottom, ymin);
        draw_line(file, top, ymax, xmax, right);
      } else {
        draw_line(file, xmin, left, top, ymax);
        draw_line(file, xmax, right, bottom, ymin);
      }
      break;
    case 6:
      draw_line(file, bottom, ymin, top, ymax);
      break;
    case 7:
      draw_line(file, xmin, left, top, ymax);
      break;
    case 8:
      draw_line(file, top, ymax, xmin, left);
      break;
    case 9:
      draw_line(file, top, ymax, bottom, ymin);
      break;
    case 10:
      if (f(xcenter, ycenter) > 0) {
        draw_line(file, top, ymax, xmin, left);
        draw_line(file, bottom, ymin, xmax, right);
      } else {
        draw_line(file, top, ymax, xmax, right);
        draw_line(file, bottom, ymin, xmin, left);
      }
      break;
    case 11:
      draw_line(file, top, ymax, xmax, right);
      break;
    case 12:
      draw_line(file, xmax, right, xmin, left);
      break;
    case 13:
      draw_line(file, xmax, right, bottom, ymin);
      break;
    case 14:
      draw_line(file, bottom, ymin, xmin, left);
      break;
    }
  }

  void divide(fstream &file, double precision, bool draw_boundaries = true) {
    double width = xmax - xmin;
    double height = ymax - ymin;

    if (width <= precision || height <= precision) {
      if (draw_boundaries) {
        draw_cell(file);
      }
      matching_squares(file);
      return;
    }

    if (should_split()) {
      double xcenter = (xmin + xmax) / 2;
      double ycenter = (ymin + ymax) / 2;

      this->is_leaf = false;

      this->NE = new QTNode(xcenter, ycenter, xmax, ymax, f);
      this->NO = new QTNode(xmin, ycenter, xcenter, ymax, f);
      this->SE = new QTNode(xcenter, ymin, xmax, ycenter, f);
      this->SO = new QTNode(xmin, ymin, xcenter, ycenter, f);

      if (draw_boundaries) {
        draw_cell(file);
      }

      this->NE->divide(file, precision, draw_boundaries);
      this->NO->divide(file, precision, draw_boundaries);
      this->SE->divide(file, precision, draw_boundaries);
      this->SO->divide(file, precision, draw_boundaries);
    } else {
      matching_squares(file);
    }
  }
};

class QTMarchingSquares {
private:
  double xmin, ymin, xmax, ymax, precision;
  string filename;
  function<double(double, double)> f;
  QTNode *root;
  bool drawQT;

public:
  QTMarchingSquares(function<double(double, double)> func, const string &fname, double x_min, double y_min, double x_max, double y_max, double prec, bool draw_boundaries = true)
      : f(func), xmin(x_min), ymin(y_min), xmax(x_max), ymax(y_max),
        precision(prec), filename(fname), drawQT(draw_boundaries) {
    root = new QTNode(xmin, ymin, xmax, ymax, f);
  }

  ~QTMarchingSquares() {
    delete root;
  }

  void generate() {
    fstream file(filename, ios::app);
    if (!file.is_open()) {
      cerr << "Error: No se pudo abrir el archivo " << filename << endl;
      return;
    }

    root->divide(file, precision, drawQT);
    file.close();
  }
};

class EPS {
private:
  string filename;

public:
  EPS(const string &fname) : filename(fname) {}

  void print_header(double xmin, double ymin, double xmax, double ymax) {
    ofstream file(filename);

    if (!file.is_open()) {
      cerr << "Error: No se pudo crear el archivo " << filename << endl;
      return;
    }

    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: " << xmin << " " << ymin << " " << xmax << " " << ymax << "\n";
    file << "%%Creator: C++ EPS Generator\n";
    file << "%%Title: Adaptive Marching Squares with Quadtree\n";
    file << "%%EndComments\n\n";

    file << "% Configuración general\n";
    file << "0.5 setlinewidth\n\n";

    file << "% Fondo blanco\n";
    file << "1 1 1 setrgbcolor\n";
    file << "newpath\n";
    file << xmin << " " << ymin << " moveto\n";
    file << xmax << " " << ymin << " lineto\n";
    file << xmax << " " << ymax << " lineto\n";
    file << xmin << " " << ymax << " lineto\n";
    file << "closepath\n";
    file << "fill\n\n";

    file << "% Dibujar borde del área\n";
    file << "0.3 0.3 0.3 setrgbcolor\n";
    file << "1 setlinewidth\n";
    file << "newpath\n";
    file << xmin << " " << ymin << " moveto\n";
    file << xmax << " " << ymin << " lineto\n";
    file << xmax << " " << ymax << " lineto\n";
    file << xmin << " " << ymax << " lineto\n";
    file << "closepath\n";
    file << "stroke\n\n";

    file << "% Configuración para las curvas de nivel\n";
    file << "1 0 0 setrgbcolor\n";
    file << "0.5 setlinewidth\n";
    file << "[] 0 setdash\n";
    file.close();
  }

  void print_footer() {
    ofstream file(filename, ios::app);
    file << "\nshowpage\n";
    file << "%%EOF\n";
    file.close();
  }
};

void draw_curve(function<double(double, double)> f, const string &filename,
                double xmin, double ymin, double xmax, double ymax, double precision) {
  EPS eps(filename);
  eps.print_header(xmin, ymin, xmax, ymax);

  bool draw_quadtree = true; // Cambiar a false para no mostrar el quadtree
  QTMarchingSquares ms(f, filename, xmin, ymin, xmax, ymax, precision, draw_quadtree);
  ms.generate();

  eps.print_footer();
}

int main() {
  auto circle = [](double x, double y) {
    double dx = x - 200;
    double dy = y - 200;
    return dx * dx + dy * dy - 100 * 100;
  };

  auto hyperbola = [](double x, double y) {
    double dx = x - 200;
    double dy = y - 200;
    return (dx * dx) / (15 * 15) - (dy * dy) / (10 * 10) - 1;
  };

  auto complex_curve = [](double x, double y) {
    double dx = x - 200;
    double dy = y - 200;
    return dx * dx * dx - 3 * dx * dy * dy - 50000;
  };

  auto wavy = [](double x, double y) {
    return y - 200 - 30 * sin(x * 0.05);
  };

  cout << "Generando archivos EPS con quadtree..." << endl;

  draw_curve(circle, "circle_low.eps", 0, 0, 400, 400, 20.0);
  draw_curve(hyperbola, "hyperbola_low.eps", 0, 0, 400, 400, 20.0);
  draw_curve(complex_curve, "complex_curve_low.eps", 0, 0, 400, 400, 20.0);
  draw_curve(wavy, "wavy_low.eps", 0, 0, 400, 400, 20.0);

  draw_curve(circle, "circle_med.eps", 0, 0, 400, 400, 5.0);
  draw_curve(hyperbola, "hyperbola_med.eps", 0, 0, 400, 400, 5.0);
  draw_curve(complex_curve, "complex_curve_med.ps", 0, 0, 400, 400, 5.0);
  draw_curve(wavy, "wavy_med.eps", 0, 0, 400, 400, 20.0);

  draw_curve(circle, "circle_high.eps", 0, 0, 400, 400, 1.0);
  draw_curve(hyperbola, "hyperbola_high.eps", 0, 0, 400, 400, 1.0);
  draw_curve(complex_curve, "complex_curve_high.eps", 0, 0, 400, 400, 1.0);
  draw_curve(wavy, "wavy_high.eps", 0, 0, 400, 400, 20.0);

  cout << "\nArchivos EPS generados exitosamente!" << endl;

  return 0;
}