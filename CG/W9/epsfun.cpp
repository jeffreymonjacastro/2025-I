#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class MarchingSquaresEPS {
private:
  std::string filename;
  int gridSize;
  double squareSize;

public:
  MarchingSquaresEPS(const std::string &fname, int grid = 4, double size = 80)
      : filename(fname), gridSize(grid), squareSize(size) {}

  void generateAllCases() {
    std::ofstream file(filename);

    if (!file.is_open()) {
      std::cerr << "Error: No se pudo crear el archivo " << filename
                << std::endl;
      return;
    }

    // Cabecera EPS
    double totalWidth = gridSize * (squareSize + 20) + 20;
    double totalHeight = gridSize * (squareSize + 40) + 40;

    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: 0 0 " << (int)totalWidth << " " << (int)totalHeight
         << "\n";
    file << "%%Creator: Marching Squares Generator\n";
    file << "%%Title: 16 Casos de Marching Squares\n";
    file << "%%EndComments\n\n";

    // Definir funciones PostScript útiles
    writePostScriptFunctions(file);

    // Generar los 16 casos
    for (int caseNum = 0; caseNum < 16; caseNum++) {
      int row = caseNum / gridSize;
      int col = caseNum % gridSize;

      double x = 20 + col * (squareSize + 20);
      double y = totalHeight - 60 - row * (squareSize + 40);

      drawMarchingSquareCase(file, caseNum, x, y);
    }

    file << "\nshowpage\n%%EOF\n";
    file.close();

    std::cout << "Archivo EPS creado: " << filename << std::endl;
    std::cout << "Contiene los 16 casos de Marching Squares" << std::endl;
  }

private:
  void writePostScriptFunctions(std::ofstream &file) {
    file << "% Función para dibujar un punto\n";
    file << "/drawpoint {\n";
    file << "  % x y radius color -> draws point\n";
    file << "  setgray newpath 3 1 roll 2 copy 3 -1 roll 0 360 arc fill\n";
    file << "} def\n\n";

    file << "% Función para dibujar línea\n";
    file << "/drawline {\n";
    file << "  % x1 y1 x2 y2 width -> draws line\n";
    file << "  setlinewidth newpath moveto lineto stroke\n";
    file << "} def\n\n";

    file << "% Función para dibujar cuadrado base\n";
    file << "/drawsquare {\n";
    file << "  % x y size -> draws square outline\n";
    file << "  0.3 setgray 1 setlinewidth newpath\n";
    file << "  3 copy moveto\n";
    file << "  2 copy 0 exch rlineto\n";
    file << "  2 copy exch 0 exch rlineto\n";
    file << "  0 exch neg rlineto\n";
    file << "  closepath stroke\n";
    file << "} def\n\n";

    file << "% Función para texto\n";
    file << "/drawtext {\n";
    file << "  % x y string -> draws text\n";
    file << "  0 setgray /Helvetica 10 selectfont moveto show\n";
    file << "} def\n\n";
  }

  void drawMarchingSquareCase(std::ofstream &file, int caseNum, double x,
                              double y) {
    // Extraer bits para determinar signos en cada vértice
    bool topLeft = (caseNum & 8) != 0;     // bit 3
    bool topRight = (caseNum & 4) != 0;    // bit 2
    bool bottomRight = (caseNum & 2) != 0; // bit 1
    bool bottomLeft = (caseNum & 1) != 0;  // bit 0

    file << "% Case " << caseNum << "\n";

    // Dibujar cuadrado base
    file << x << " " << y << " " << squareSize << " drawsquare\n";

    // Coordenadas de los vértices
    double corners[4][2] = {
        {x, y},                           // bottom-left
        {x + squareSize, y},              // bottom-right
        {x + squareSize, y + squareSize}, // top-right
        {x, y + squareSize}               // top-left
    };

    // Dibujar puntos en las esquinas
    file << corners[3][0] << " " << corners[3][1] << " 3 "
         << (topLeft ? "0" : "1") << " drawpoint\n"; // top-left
    file << corners[2][0] << " " << corners[2][1] << " 3 "
         << (topRight ? "0" : "1") << " drawpoint\n"; // top-right
    file << corners[1][0] << " " << corners[1][1] << " 3 "
         << (bottomRight ? "0" : "1") << " drawpoint\n"; // bottom-right
    file << corners[0][0] << " " << corners[0][1] << " 3 "
         << (bottomLeft ? "0" : "1") << " drawpoint\n"; // bottom-left

    // Dibujar las líneas según el caso
    drawLinesForCase(file, caseNum, x, y);

    // Etiqueta del caso
    file << x << " " << (y - 15) << " (Case " << caseNum << ") drawtext\n\n";
  }

  void drawLinesForCase(std::ofstream &file, int caseNum, double x, double y) {
    double half = squareSize / 2.0;
    double full = squareSize;

    // Puntos medios de los lados
    double midPoints[4][2] = {
        {x + half, y},        // bottom middle
        {x + full, y + half}, // right middle
        {x + half, y + full}, // top middle
        {x, y + half}         // left middle
    };

    // Color de línea (azul cian)
    file << "0 0.7 1 setrgbcolor 2.5 setlinewidth\n";

    switch (caseNum) {
    case 0: // No lines
      break;

    case 1: // Bottom-left corner
      file << midPoints[3][0] << " " << midPoints[3][1] << " "
           << midPoints[0][0] << " " << midPoints[0][1] << " stroke\n";
      file << "newpath " << midPoints[3][0] << " " << midPoints[3][1]
           << " moveto " << midPoints[0][0] << " " << midPoints[0][1]
           << " lineto stroke\n";
      break;

    case 2: // Bottom-right corner
      file << "newpath " << midPoints[0][0] << " " << midPoints[0][1]
           << " moveto " << midPoints[1][0] << " " << midPoints[1][1]
           << " lineto stroke\n";
      break;

    case 3: // Bottom edge
      file << "newpath " << midPoints[3][0] << " " << midPoints[3][1]
           << " moveto " << midPoints[1][0] << " " << midPoints[1][1]
           << " lineto stroke\n";
      break;

    case 4: // Top-right corner
      file << "newpath " << midPoints[1][0] << " " << midPoints[1][1]
           << " moveto " << midPoints[2][0] << " " << midPoints[2][1]
           << " lineto stroke\n";
      break;

    case 5: // Diagonal (saddle point)
      file << "newpath " << midPoints[3][0] << " " << midPoints[3][1]
           << " moveto " << midPoints[0][0] << " " << midPoints[0][1]
           << " lineto stroke\n";
      file << "newpath " << midPoints[1][0] << " " << midPoints[1][1]
           << " moveto " << midPoints[2][0] << " " << midPoints[2][1]
           << " lineto stroke\n";
      break;

    case 6: // Right edge
      file << "newpath " << midPoints[0][0] << " " << midPoints[0][1]
           << " moveto " << midPoints[2][0] << " " << midPoints[2][1]
           << " lineto stroke\n";
      break;

    case 7: // L-shape (bottom-right)
      file << "newpath " << midPoints[3][0] << " " << midPoints[3][1]
           << " moveto " << midPoints[2][0] << " " << midPoints[2][1]
           << " lineto stroke\n";
      break;

    case 8: // Top-left corner
      file << "newpath " << midPoints[2][0] << " " << midPoints[2][1]
           << " moveto " << midPoints[3][0] << " " << midPoints[3][1]
           << " lineto stroke\n";
      break;

    case 9: // Left edge
      file << "newpath " << midPoints[2][0] << " " << midPoints[2][1]
           << " moveto " << midPoints[0][0] << " " << midPoints[0][1]
           << " lineto stroke\n";
      break;

    case 10: // Diagonal (saddle point, opposite)
      file << "newpath " << midPoints[2][0] << " " << midPoints[2][1]
           << " moveto " << midPoints[3][0] << " " << midPoints[3][1]
           << " lineto stroke\n";
      file << "newpath " << midPoints[0][0] << " " << midPoints[0][1]
           << " moveto " << midPoints[1][0] << " " << midPoints[1][1]
           << " lineto stroke\n";
      break;

    case 11: // L-shape (bottom-left)
      file << "newpath " << midPoints[2][0] << " " << midPoints[2][1]
           << " moveto " << midPoints[1][0] << " " << midPoints[1][1]
           << " lineto stroke\n";
      break;

    case 12: // Top edge
      file << "newpath " << midPoints[3][0] << " " << midPoints[3][1]
           << " moveto " << midPoints[1][0] << " " << midPoints[1][1]
           << " lineto stroke\n";
      break;

    case 13: // L-shape (top-right)
      file << "newpath " << midPoints[0][0] << " " << midPoints[0][1]
           << " moveto " << midPoints[2][0] << " " << midPoints[2][1]
           << " lineto stroke\n";
      break;

    case 14: // L-shape (top-left)
      file << "newpath " << midPoints[0][0] << " " << midPoints[0][1]
           << " moveto " << midPoints[3][0] << " " << midPoints[3][1]
           << " lineto stroke\n";
      break;

    case 15: // No lines (all positive)
      break;
    }
  }
};

int main() {
  try {
    MarchingSquaresEPS generator("marching_squares_16_cases.eps");
    generator.generateAllCases();

    std::cout << "\n¡Archivo generado exitosamente!" << std::endl;
    std::cout << "Puedes visualizarlo con un visor PostScript o convertirlo:"
              << std::endl;
    std::cout << "- ps2pdf marching_squares_16_cases.eps output.pdf"
              << std::endl;
    std::cout << "- convert marching_squares_16_cases.eps output.png"
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}