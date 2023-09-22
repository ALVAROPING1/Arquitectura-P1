// Programa con la aplicación. Contendrá un único archivo fuente

#include <span>
#include <vector>
#include <gsl/gsl>

#include "sim/progargs.hpp"

int main (int argc, char **argv) {
  Fluid::ArgumentParser argp{{argv, static_cast<std::size_t>(argc)}};
  if (!argp)
    return argp.error();
}
