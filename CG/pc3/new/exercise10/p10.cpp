#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <memory>
#include <limits>

#include "json.hpp"
#include "exprtk.hpp"

using namespace std;
using json = nlohmann::json;

// ===================================================================
// PARSER DE FUNCIONES (EXPRTK) - SIN CAMBIOS
// ===================================================================
class ParserFunc
{
public:
    ParserFunc(const string& expression_string) {
        if (expression_string.empty()) { throw runtime_error("Error: Se intento compilar una expresion vacia."); }
        m_symbol_table.add_variable("x", m_x);
        m_symbol_table.add_variable("y", m_y);
        m_symbol_table.add_variable("z", m_z);
        m_function.register_symbol_table(m_symbol_table);
        exprtk::parser<double> parser;
        if (!parser.compile(expression_string, m_function)) { throw runtime_error("Error de sintaxis en la expresion: " + parser.error()); }
    }
    double feval(double x, double y, double z) {
        m_x = x; m_y = y; m_z = z;
        return m_function.value();
    }
private:
    double m_x = 0.0, m_y = 0.0, m_z = 0.0;
    exprtk::symbol_table<double> m_symbol_table;
    exprtk::expression<double> m_function;
};

// ===================================================================
// PARSER DEL ÁRBOL JSON Y EVALUADOR CSG - SIN CAMBIOS
// ===================================================================
struct Node {
    string op;
    unique_ptr<ParserFunc> function;
    vector<Node> childs;
    string sfunction;
};

class ParserJson {
private:
    Node root_node;
    Node Parse_all_tree(const json& j_node) {
        Node c_node;
        if (j_node.contains("op")) { c_node.op = j_node.at("op").get<string>(); }
        if (j_node.contains("function")) {
            string func_str = j_node.at("function").get<string>();
            if (!func_str.empty()) {
                c_node.function = make_unique<ParserFunc>(func_str);
                c_node.sfunction = func_str;
            }
        }
        if (j_node.contains("childs") && j_node.at("childs").is_array()) {
            for (const auto& j_child : j_node.at("childs")) {
                c_node.childs.push_back(Parse_all_tree(j_child));
            }
        }
        return c_node;
    }
    double eval_tree(const Node& current_node, double x, double y, double z) {
        if (current_node.op == "shape") {
            if (current_node.function) return current_node.function->feval(x, y, z);
            return numeric_limits<double>::infinity();
        }
        if (current_node.op == "union") {
            double min_val = numeric_limits<double>::infinity();
            for (const auto& child : current_node.childs) min_val = min(min_val, eval_tree(child, x, y, z));
            return min_val;
        }
        if (current_node.op == "intersection") {
            double max_val = -numeric_limits<double>::infinity();
            for (const auto& child : current_node.childs) max_val = max(max_val, eval_tree(child, x, y, z));
            return max_val;
        }
        if (current_node.op == "difference") {
            if (current_node.childs.size() < 1) return numeric_limits<double>::infinity();
            double base_val = eval_tree(current_node.childs[0], x, y, z);
            for (size_t i = 1; i < current_node.childs.size(); ++i) {
                base_val = max(base_val, -eval_tree(current_node.childs[i], x, y, z));
            }
            return base_val;
        }
        return numeric_limits<double>::infinity();
    }
public:
    ParserJson(const string& json_string) {
        json data = json::parse(json_string);
        root_node = Parse_all_tree(data);
    }
    double eval(double x, double y, double z) {
        return eval_tree(root_node, x, y, z);
    }
};

// ===================================================================
// MOTOR DE DIBUJO (MARCHING SQUARES ADAPTATIVO) - CORREGIDO
// ===================================================================
class MS_adaptative {
public:
    MS_adaptative() {}

    void draw_line(fstream& file, double init_x, double init_y, double end_x, double end_y) {
        file << init_x << " " << init_y << " moveto" << endl;
        file << end_x << " " << end_y << " lineto" << endl;
        file << "stroke" << endl;
    }

    double find_intersection(double x1, double y1, double x2, double y2, const function<double(double, double)>& func, bool is_horizontal) {
        double f1 = func(x1, y1);
        for (int i = 0; i < 10; i++) {
            double x_mid = (x1 + x2) / 2.0;
            double y_mid = (y1 + y2) / 2.0;
            double f_mid = func(x_mid, y_mid);
            if (abs(f_mid) < 1e-6) return is_horizontal ? x_mid : y_mid;
            if (f1 * f_mid < 0) { x2 = x_mid; y2 = y_mid; }
            else { x1 = x_mid; y1 = y_mid; f1 = f_mid; }
        }
        return is_horizontal ? (x1 + x2) / 2.0 : (y1 + y2) / 2.0;
    }

    void draw(fstream& file, const function<double(double, double)>& func, double x_min, double y_min, double x_max, double y_max) {
        int to_case = (func(x_min, y_min) < 0) * 8 + (func(x_max, y_min) < 0) * 4 + (func(x_max, y_max) < 0) * 2 + (func(x_min, y_max) < 0) * 1;
        double left = find_intersection(x_min, y_min, x_min, y_max, func, false);
        double right = find_intersection(x_max, y_min, x_max, y_max, func, false);
        double bottom = find_intersection(x_min, y_min, x_max, y_min, func, true);
        double top = find_intersection(x_min, y_max, x_max, y_max, func, true);
        file << "newpath" << endl;
        switch (to_case) {
            case 1: draw_line(file, x_min, left, top, y_max); break;
            case 2: draw_line(file, x_max, right, top, y_max); break;
            case 3: draw_line(file, x_min, left, x_max, right); break;
            case 4: draw_line(file, x_max, right, bottom, y_min); break;
            case 5: draw_line(file, x_min, left, bottom, y_min); draw_line(file, x_max, right, top, y_max); break;
            case 6: draw_line(file, bottom, y_min, top, y_max); break;
            case 7: draw_line(file, x_min, left, bottom, y_min); break;
            case 8: draw_line(file, x_min, left, bottom, y_min); break;
            case 9: draw_line(file, bottom, y_min, top, y_max); break;
            case 10: draw_line(file, x_min, left, top, y_max); draw_line(file, x_max, right, bottom, y_min); break;
            case 11: draw_line(file, x_max, right, bottom, y_min); break;
            case 12: draw_line(file, x_min, left, x_max, right); break;
            case 13: draw_line(file, x_max, right, top, y_max); break;
            case 14: draw_line(file, x_min, left, top, y_max); break;
        }
    }

    // --- FUNCIÓN 'should_split' CORREGIDA ---
    bool should_split(double x_mn, double y_mn, double x_m, double y_m, const function<double(double, double)>& func) {
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

    void split_grid(fstream& file, double x_min, double y_min, double x_max, double y_max, double precision, const function<double(double, double)>& func) {
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

class EPS {
private:
    double h_, w_;
public:
    EPS(double w, double h) : w_(w), h_(h) {}
    void header(fstream& file) {
        file << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
        file << "%%BoundingBox: 0 0 " << (int)w_ << " " << (int)h_ << endl;
        file << "0.5 setlinewidth" << endl;
    }
};

void draw_curve(const function<double(double, double)>& f, string output_filename, double xmin, double ymin, double xmax, double ymax, double precision) {
    fstream file(output_filename, ios::out);
    EPS eps(xmax - xmin, ymax - ymin);
    eps.header(file);
    MS_adaptative ms;
    ms.split_grid(file, xmin, ymin, xmax, ymax, precision, f);
    file.close();
}

// ===================================================================
// MAIN
// ===================================================================
int main() {
    string json_string = R"({
        "op": "union",
        "childs": [
            {
                "op": "shape",
                "function": "(x-220)^2 + (y-200)^2 - 80^2"
            },
            {
                "op": "shape",
                "function": "(x-180)^2 + (y-200)^2 - 80^2"
            }
        ]
    })";

    try {
        ParserJson scene_parser(json_string);
        
        auto csg_function = [&](double x, double y) -> double {
            return scene_parser.eval(x, y, 0.0);
        };

        cout << "Generando el resultado de la operacion CSG..." << endl;
        draw_curve(csg_function, "csg_output.eps", 0, 0, 400, 400, 1.0);
        cout << "Archivo 'csg_output.eps' generado." << endl;

    } catch (const exception& e) {
        cerr << "Ha ocurrido un error fatal: " << e.what() << endl;
        return 1;
    }

    return 0;
}