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

      /// Devuelve si ha habido algún error
      constexpr explicit operator bool() const { return error_ == 0; }

      /// Devuelve el código de error
      constexpr int error() const { return error_; }

      /// Devuelve el número de pasos de tiempo a ejecutar
      constexpr int step_count() const { return steps_; }

      /// Devuelve el objeto de archivos de IO
      FileIO & file() { return file_; }

    private:
      int error_;    // Código de error
      int steps_;    // Número de pasos de tiempo a ejecutar
      FileIO file_;  // Objeto de archivos de IO

      /// Establece un código de error, enviando un mensaje de error por `std::cerr`
      template <typename... Args>
      constexpr void error(int code, Args... args) {
        error_ = code;
        IO::put_line(std::cerr, args...);
      }

      /// Comprueba si el código de error es el índicado, enviando un mensaje de error por
      /// `std::cerr` en caso afirmativo
      template <typename... Args>
      constexpr void check(int code, Args... args) {
        if (error_ == code) { IO::put_line(std::cerr, args...); }
      }
  };

}  // namespace Fluid
#endif  // SIM_PROGARGS_HPP
