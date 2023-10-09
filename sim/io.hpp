#ifndef __IO_HPP
#define __IO_HPP

#include <iostream>

namespace Fluid::IO {

  // Write Text

  template <typename T>
  concept Printable = requires(T t) { std::cout << t; };

  template <typename... Args>
  constexpr void put(std::ostream & out, Args... args);

  constexpr void put(std::ostream &) { }

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
  void read(std::istream & in, Args &... args);

  // Paso recursivo
  template <typename T, typename... Args>
  void read(std::istream & in, T & result, Args &... args);

  // Casos base
  template <typename T>
    requires std::is_arithmetic<T>::value
  void read(std::istream & in, T & result) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    in.read(reinterpret_cast<char *>(&result), sizeof(T));
  }

  template <typename T, typename... Args>
  void read(std::istream & in, T & result, Args &... args) {
    read(in, result);
    read(in, args...);
  }

  // Función general
  template <typename... Args>
  void write(std::ostream & out, Args const &... args);

  // Paso recursivo
  template <typename T, typename... Args>
  void write(std::ostream & out, T const & result, Args const &... args);

  // Casos base
  template <typename T>
    requires std::is_arithmetic<T>::value
  void write(std::ostream & out, T const & result) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    out.write(reinterpret_cast<char const *>(&result), sizeof(T));
  }

  template <typename T, typename... Args>
  void write(std::ostream & out, T const & result, Args const &... args) {
    write(out, result);
    write(out, args...);
  }

}  // namespace Fluid::IO

#endif  //__IO_HPP
