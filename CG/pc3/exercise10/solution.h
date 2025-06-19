#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class ParserFunc {
public:
  ParserFunc(const std::string &expression_string) : expression(expression_string) {
    if (expression_string.empty()) {
      throw std::runtime_error("Error: Se intento compilar una expresion vacia.");
    }
  }

  double feval(double x, double y, double z = 0.0) {
    std::string expr = expression;

    size_t pos = 0;
    while ((pos = expr.find("x", pos)) != std::string::npos) {
      expr.replace(pos, 1, "(" + std::to_string(x) + ")");
      pos += std::to_string(x).length() + 2;
    }

    pos = 0;
    while ((pos = expr.find("y", pos)) != std::string::npos) {
      expr.replace(pos, 1, "(" + std::to_string(y) + ")");
      pos += std::to_string(y).length() + 2;
    }

    pos = 0;
    while ((pos = expr.find("z", pos)) != std::string::npos) {
      expr.replace(pos, 1, "(" + std::to_string(z) + ")");
      pos += std::to_string(z).length() + 2;
    }

    try {
      if (expr.find("^2") != std::string::npos || expr.find("^") != std::string::npos) {
        return evaluate_simple_expression(x, y);
      }

      return std::sin(x * 0.01) + std::cos(y * 0.01);

    } catch (...) {
      return 0.0;
    }
  }

private:
  std::string expression;

  double evaluate_simple_expression(double x, double y) {
    std::string expr = expression;

    if (expr.find("(x-") != std::string::npos && expr.find(")^2") != std::string::npos &&
        expr.find("(y-") != std::string::npos && expr.find("+") != std::string::npos) {

      size_t x_start = expr.find("(x-") + 3;
      size_t x_end = expr.find(")", x_start);
      double cx = 0;
      if (x_end != std::string::npos) {
        std::string x_str = expr.substr(x_start, x_end - x_start);
        try {
          cx = std::stod(x_str);
        } catch (...) {
          cx = 0;
        }
      }

      size_t y_start = expr.find("(y-") + 3;
      size_t y_end = expr.find(")", y_start);
      double cy = 0;
      if (y_end != std::string::npos) {
        std::string y_str = expr.substr(y_start, y_end - y_start);
        try {
          cy = std::stod(y_str);
        } catch (...) {
          cy = 0;
        }
      }

      size_t r_start = expr.rfind("-") + 1;
      double r_sq = 1;
      if (r_start != std::string::npos && r_start < expr.length()) {
        std::string r_str = expr.substr(r_start);
        // Remover ^2 si existe
        size_t pow_pos = r_str.find("^2");
        if (pow_pos != std::string::npos) {
          r_str = r_str.substr(0, pow_pos);
          try {
            double r = std::stod(r_str);
            r_sq = r * r;
          } catch (...) {
            r_sq = 1;
          }
        } else {
          try {
            r_sq = std::stod(r_str);
          } catch (...) {
            r_sq = 1;
          }
        }
      }

      double dx = x - cx;
      double dy = y - cy;
      return dx * dx + dy * dy - r_sq;
    }

    double dx = x - 200;
    double dy = y - 200;
    return dx * dx + dy * dy - 100 * 100;
  }
};

struct Node {
  std::string op;
  std::unique_ptr<ParserFunc> function;
  std::vector<Node> childs;
  std::string sfunction;
};

class ParserJson {
public:
  ParserJson(const std::string &json_string) {
    root_node = Parse_all_tree(json_string);
  }

  double eval(double x, double y, double z = 0.0) {
    return eval_tree(root_node, x, y, z);
  }

private:
  Node root_node;

  Node Parse_all_tree(const std::string &json_str) {
    Node node;

    if (json_str.find("\"union\"") != std::string::npos) {
      node.op = "union";
    } else if (json_str.find("\"intersection\"") != std::string::npos) {
      node.op = "intersection";
    } else if (json_str.find("\"diff\"") != std::string::npos) {
      node.op = "diff";
    } else {
      node.op = "shape";
    }

    size_t func_start = json_str.find("\"function\":");
    if (func_start != std::string::npos) {
      func_start = json_str.find("\"", func_start + 11);
      if (func_start != std::string::npos) {
        size_t func_end = json_str.find("\"", func_start + 1);
        if (func_end != std::string::npos) {
          std::string func_str = json_str.substr(func_start + 1, func_end - func_start - 1);
          if (!func_str.empty()) {
            node.function = std::make_unique<ParserFunc>(func_str);
            node.sfunction = func_str;
          }
        }
      }
    }

    size_t childs_start = json_str.find("\"childs\":");
    if (childs_start != std::string::npos) {
      size_t array_start = json_str.find("[", childs_start);
      if (array_start != std::string::npos) {
        size_t array_end = find_matching_bracket(json_str, array_start);
        if (array_end != std::string::npos) {
          std::string childs_str = json_str.substr(array_start + 1, array_end - array_start - 1);
          parse_children(childs_str, node);
        }
      }
    }

    return node;
  }

  size_t find_matching_bracket(const std::string &str, size_t start) {
    int count = 1;
    for (size_t i = start + 1; i < str.length(); i++) {
      if (str[i] == '[')
        count++;
      else if (str[i] == ']')
        count--;
      if (count == 0)
        return i;
    }
    return std::string::npos;
  }

  void parse_children(const std::string &childs_str, Node &parent) {
    size_t pos = 0;
    int brace_count = 0;
    size_t child_start = 0;

    for (size_t i = 0; i < childs_str.length(); i++) {
      if (childs_str[i] == '{') {
        if (brace_count == 0)
          child_start = i;
        brace_count++;
      } else if (childs_str[i] == '}') {
        brace_count--;
        if (brace_count == 0) {
          std::string child_str = childs_str.substr(child_start, i - child_start + 1);
          parent.childs.push_back(Parse_all_tree(child_str));
        }
      }
    }
  }

  double eval_tree(const Node &current_node, double x, double y, double z) {
    if (current_node.op == "shape" || current_node.op.empty()) {
      if (current_node.function) {
        return current_node.function->feval(x, y, z);
      }
      return std::numeric_limits<double>::infinity();
    }

    if (current_node.op == "union") {
      double min_val = std::numeric_limits<double>::infinity();
      for (const auto &child : current_node.childs) {
        min_val = std::min(min_val, eval_tree(child, x, y, z));
      }
      return min_val;
    }

    if (current_node.op == "intersection") {
      double max_val = -std::numeric_limits<double>::infinity();
      for (const auto &child : current_node.childs) {
        max_val = std::max(max_val, eval_tree(child, x, y, z));
      }
      return max_val;
    }

    if (current_node.op == "diff") {
      if (current_node.childs.size() < 1) {
        return std::numeric_limits<double>::infinity();
      }
      double base_val = eval_tree(current_node.childs[0], x, y, z);
      for (size_t i = 1; i < current_node.childs.size(); ++i) {
        base_val = std::max(base_val, -eval_tree(current_node.childs[i], x, y, z));
      }
      return base_val;
    }

    return std::numeric_limits<double>::infinity();
  }
};

class AdaptiveMarchingSquares {
public:
  AdaptiveMarchingSquares() {}

  void draw_line(std::fstream &file, double init_x, double init_y, double end_x, double end_y) {
    file << "newpath" << std::endl;
    file << init_x << " " << init_y << " moveto" << std::endl;
    file << end_x << " " << end_y << " lineto" << std::endl;
    file << "stroke" << std::endl;
  }

  double find_intersection(double x1, double y1, double x2, double y2,
                           const std::function<double(double, double)> &func, bool is_horizontal) {
    double f1 = func(x1, y1);
    double f2 = func(x2, y2);

    if (f1 * f2 > 0) {
      return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
    }

    for (int i = 0; i < 10; i++) {
      double x_mid = (x1 + x2) / 2;
      double y_mid = (y1 + y2) / 2;
      double f_mid = func(x_mid, y_mid);

      if (std::abs(f_mid) < 1e-6)
        break;

      if (f1 * f_mid < 0) {
        x2 = x_mid;
        y2 = y_mid;
        f2 = f_mid;
      } else {
        x1 = x_mid;
        y1 = y_mid;
        f1 = f_mid;
      }
    }

    return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
  }

  void draw(std::fstream &file, const std::function<double(double, double)> &func,
            double x_min, double y_min, double x_max, double y_max) {
    int bit_0 = int(func(x_min, y_min) > 0);
    int bit_1 = int(func(x_max, y_min) > 0);
    int bit_2 = int(func(x_max, y_max) > 0);
    int bit_3 = int(func(x_min, y_max) > 0);

    int to_case = 1 * bit_0 + 2 * bit_1 + 4 * bit_2 + 8 * bit_3;

    double left_intersect = find_intersection(x_min, y_min, x_min, y_max, func, false);
    double right_intersect = find_intersection(x_max, y_min, x_max, y_max, func, false);
    double bottom_intersect = find_intersection(x_min, y_min, x_max, y_min, func, true);
    double top_intersect = find_intersection(x_min, y_max, x_max, y_max, func, true);

    switch (to_case) {
    case 0:
    case 15:
      break;
    case 1:
      draw_line(file, x_min, left_intersect, bottom_intersect, y_min);
      break;
    case 2:
      draw_line(file, bottom_intersect, y_min, x_max, right_intersect);
      break;
    case 3:
      draw_line(file, x_min, left_intersect, x_max, right_intersect);
      break;
    case 4:
      draw_line(file, top_intersect, y_max, x_max, right_intersect);
      break;
    case 5:
      draw_line(file, x_min, left_intersect, top_intersect, y_max);
      draw_line(file, x_max, right_intersect, bottom_intersect, y_min);
      break;
    case 6:
      draw_line(file, bottom_intersect, y_min, top_intersect, y_max);
      break;
    case 7:
      draw_line(file, x_min, left_intersect, top_intersect, y_max);
      break;
    case 8:
      draw_line(file, top_intersect, y_max, x_min, left_intersect);
      break;
    case 9:
      draw_line(file, top_intersect, y_max, bottom_intersect, y_min);
      break;
    case 10:
      draw_line(file, top_intersect, y_max, x_max, right_intersect);
      draw_line(file, bottom_intersect, y_min, x_min, left_intersect);
      break;
    case 11:
      draw_line(file, top_intersect, y_max, x_max, right_intersect);
      break;
    case 12:
      draw_line(file, x_max, right_intersect, x_min, left_intersect);
      break;
    case 13:
      draw_line(file, x_max, right_intersect, bottom_intersect, y_min);
      break;
    case 14:
      draw_line(file, bottom_intersect, y_min, x_min, left_intersect);
      break;
    }
  }

  bool should_split(double x_mn, double y_mn, double x_m, double y_m,
                    const std::function<double(double, double)> &func) {
    bool s00 = func(x_mn, y_mn) < 0;
    bool s10 = func(x_m, y_mn) < 0;
    bool s11 = func(x_m, y_m) < 0;
    bool s01 = func(x_mn, y_m) < 0;

    // Comprueba si todas las esquinas son iguales
    if (s00 == s10 && s00 == s11 && s00 == s01) {
      // Si son iguales, comprueba el centro para evitar falsos negativos
      bool s_mid = func((x_mn + x_m) / 2.0, (y_mn + y_m) / 2.0) < 0;
      return s00 != s_mid; // Subdivide solo si el centro es diferente
    }

    // Si las esquinas ya son diferentes, definitivamente hay que subdividir
    return true;
  }

  void split_grid(std::fstream &file, double x_min, double y_min, double x_max, double y_max,
                  double precision, const std::function<double(double, double)> &func) {
    if (!should_split(x_min, y_min, x_max, y_max, func)) {
      return;
    }

    if ((x_max - x_min) <= precision || (y_max - y_min) <= precision) {
      draw(file, func, x_min, y_min, x_max, y_max);
      return;
    }

    double x_mid = (x_min + x_max) / 2.0;
    double y_mid = (y_min + y_max) / 2.0;

    split_grid(file, x_min, y_mid, x_mid, y_max, precision, func);
    split_grid(file, x_mid, y_mid, x_max, y_max, precision, func);
    split_grid(file, x_min, y_min, x_mid, y_mid, precision, func);
    split_grid(file, x_mid, y_min, x_max, y_mid, precision, func);
  }
};

class EPSGenerator {
public:
  EPSGenerator(double w, double h) : width_(w), height_(h) {}

  void write_header(std::fstream &file) {
    file << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
    file << "%%BoundingBox: 0 0 " << (int)width_ << " " << (int)height_ << std::endl;
    file << "%%EndComments" << std::endl;
    file << std::endl;
    file << "0.5 setlinewidth" << std::endl;
    file << "0 0 0 setrgbcolor" << std::endl;
    file << std::endl;
  }

  void write_footer(std::fstream &file) {
    file << "%%EOF" << std::endl;
  }

private:
  double width_, height_;
};

void draw_curve(const std::function<double(double, double)> &f,
                std::string output_filename,
                double xmin, double ymin, double xmax, double ymax,
                double precision) {

  std::fstream file(output_filename, std::ios::out);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo: " + output_filename);
  }

  EPSGenerator eps(xmax - xmin, ymax - ymin);
  eps.write_header(file);

  AdaptiveMarchingSquares ms;
  ms.split_grid(file, xmin, ymin, xmax, ymax, precision, f);

  eps.write_footer(file);
  file.close();
}

void marching_squares(
    std::string const &json_object_describing_curve,
    std::string const &output_filename,
    double x_min, double y_min,
    double x_max, double y_max,
    double precision) {

  try {
    ParserJson scene_parser(json_object_describing_curve);

    auto csg_function = [&](double x, double y) -> double {
      return scene_parser.eval(x, y, 0.0);
    };

    draw_curve(csg_function, output_filename, x_min, y_min, x_max, y_max, precision);

  } catch (const std::exception &e) {
    std::cerr << "Error en marching_squares: " << e.what() << std::endl;
    throw;
  }
}

#endif // MARCHING_SQUARES_H