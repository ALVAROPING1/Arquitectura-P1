// Programa con la aplicación. Contendrá un único archivo fuente

#include "sim/argumentparser.hpp"
#include "sim/fileio.hpp"
#include "sim/grid.hpp"

#include <gsl/gsl>
#include <iostream>
#include <span>
#include <vector>

int main(int argc, char const ** argv) {
  // Inicializa el programa
  Fluid::ArgumentParser const argp{
      {argv, static_cast<std::size_t>(argc)}
  };
  Fluid::FileIO file{argp.in(), argp.out()};
  Fluid::Grid grid{file.np(), file.ppm()};
  // Carga el archivo de entrada
  file.load(grid);
  // Realiza las iteraciones
  for (int i = 0; i < argp.step_count(); ++i) { grid.step(); }
  file.dump(grid);
  // Escribre el archivo de salida
  return 0;
}
