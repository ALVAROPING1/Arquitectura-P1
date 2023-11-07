// Programa con la aplicación. Contendrá un único archivo fuente

#include "sim/argumentparser.hpp"
#include "sim/fileio.hpp"
#include "sim/grid.hpp"

#include <gsl/gsl>
#include <iostream>
#include <span>
#include <vector>

int main(int argc, char ** argv) {
  Fluid::ArgumentParser argp{
      {argv, static_cast<std::size_t>(argc)}
  };
  if (!argp) { return argp.error(); }
  Fluid::FileIO & file = argp.file();
  Fluid::Grid grid{file.np(), file.ppm()};
  file.load(grid);
  for (int i = 0; i < argp.step_count(); ++i) { grid.step(); }
  file.dump(grid);
  return 0;
}
