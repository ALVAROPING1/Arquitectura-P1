// Programa con la aplicación. Contendrá un único archivo fuente

#include "sim/argumentparser.hpp"

#include <gsl/gsl>
#include <span>
#include <vector>

int main(int argc, char ** argv) {
  Fluid::ArgumentParser argp{
      {argv, static_cast<std::size_t>(argc)}
  };
  if (!argp) { return argp.error(); }
}
