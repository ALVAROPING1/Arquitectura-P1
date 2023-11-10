#ifndef IO_HPP
#define IO_HPP

#include "vector3.hpp"

#include <iostream>

namespace Fluid::IO {

  // Write Text

  template <typename T>
  concept Printable = requires(T t) { std::cout << t; };

  /// Escribe un mensaje en la salida dada
  /// @param out
  /// Salida en la que escribir el mensaje
  /// @param ...
  /// Mensajes a escribir
  template <typename... Args>
    requires(Printable<Args>, ...)
  constexpr void put(std::ostream & out, Args... args) {
    (out << ... << args);
  }

  /// Escribe una línea de mensajes en la salida dada
  /// @param out
  /// Salida en la que escribir el mensaje
  /// @param ...
  /// Mensajes a escribir
  template <typename... Args>
  constexpr void put_line(std::ostream & out, Args... args) {
    put(out, args..., "\n");
  }

  // Read Binary

  // Función general
  template <typename... Args>
  bool read(std::istream & in, Args &... args);

  // Paso recursivo
  template <typename T, typename... Args>
  bool read(std::istream & in, T & result, Args &... args);

  // Casos base
  template <typename T>
    requires std::is_arithmetic_v<T>
  bool read(std::istream & in, T & result) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    return static_cast<bool>(in.read(reinterpret_cast<char *>(&result), sizeof(T)));
  }

  template <typename T>
  bool read(std::istream & in, Vector3<T> & result) {
    return read(in, result.x, result.y, result.z);
  }

  template <typename T, typename... Args>
  bool read(std::istream & in, T & result, Args &... args) {
    return read(in, result) && read(in, args...);
  }

  /// Escribe buffers en binario un archivo
  /// @param out
  /// Archivo en el que escribir los buffers
  /// @param ...
  /// Buffers a escribir
  template <typename... Args>
  void write(std::ostream & out, Args const &... args) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    (out.write(reinterpret_cast<char const *>(&args), sizeof(Args)), ...);
  }
}  // namespace Fluid::IO

#endif  // IO_HPP
