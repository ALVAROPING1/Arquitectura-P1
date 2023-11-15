// Tratamiento de argumentos para los parámetros de main()

#ifndef SIM_PROGARGS_HPP
#define SIM_PROGARGS_HPP

#include "fileio.hpp"
#include "io.hpp"

#include <gsl/gsl>
#include <iostream>
#include <span>

namespace Fluid {

  class ArgumentParser {
    public:
      /// Construye un nuevo parser de argumentos
      /// @param args
      /// Argumentos del programa. Formato esperado:
      /// `(<cmd>, <pasos>, <archivo_entrada>, <archivo_salida>)`
      ArgumentParser(std::span<gsl::czstring> args);

      /// Devuelve el número de pasos de tiempo a ejecutar
      constexpr int step_count() const { return steps_; }

      /// Devuelve el objeto de archivos de IO
      FileIO & file() { return file_; }

    private:
      int steps_;    // Número de pasos de tiempo a ejecutar
      FileIO file_;  // Objeto de archivos de IO
  };

}  // namespace Fluid
#endif  // SIM_PROGARGS_HPP
