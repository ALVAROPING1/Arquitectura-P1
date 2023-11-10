#ifndef IO_HPP
#define IO_HPP

#include "vector3.hpp"

#include <iostream>

namespace Fluid::IO {

  // Write Text

  template <typename T>
  concept Printable = requires(T t) { std::cout << t; };

  template <typename... Args>
  constexpr void put(std::ostream & out, Args... args);

  constexpr void put(std::ostream & /*unused*/) { }

  template <typename Arg, typename... Args>
    requires Printable<Arg>
  constexpr void put(std::ostream & out, Arg arg, Args... args) {
    out << arg;
    put(out, args...);
  }

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

  // Función general
  template <typename... Args>
  void write(std::ostream & out, Args const &... args);

  // Paso recursivo
  template <typename T, typename... Args>
  void write(std::ostream & out, T const & result, Args const &... args);

  // Casos base
  template <typename T>
    requires std::is_arithmetic_v<T>
  void write(std::ostream & out, T const & result) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    out.write(reinterpret_cast<char const *>(&result), sizeof(T));
  }

  template <typename T>
  void write(std::ostream & out, Vector3<T> const & result) {
    write(out, result.x, result.y, result.z);
  }

  template <typename T, typename... Args>
  void write(std::ostream & out, T const & result, Args const &... args) {
    write(out, result);
    write(out, args...);
  }

}  // namespace Fluid::IO

#endif  // IO_HPP
