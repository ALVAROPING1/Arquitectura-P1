#ifndef __VECTOR3_HPP
#define __VECTOR3_HPP

#include <array>
#include <ostream>
#include <type_traits>

namespace Fluid {
  template <class T>
    requires(std::is_arithmetic<T>::value)
  struct Vector3 {
      T x{}, y{}, z{};

      friend std::ostream & operator<<(std::ostream & out, Vector3 const & c) {
        return out << "("
                   << "x: " << c.x << ",y: " << c.y << ",z: " << c.z << ")";
      }

      // XXX: Adri ¿¿¿pa qué te sirve el Vector3 si usas std::array???
      std::array<T, 3> to_array() { return std::array<T, 3>{x, y, z}; }

      constexpr Vector3 operator+(Vector3 const & other) const {
        return Vector3{x + other.x, y + other.y, z + other.z};
      };

      constexpr Vector3 operator-(Vector3 const & other) const {
        return Vector3{x - other.x, y - other.y, z - other.z};
      };

      // XXX: Adri léete la memoria y busca que operadores se pueden necesitar
      //      Tipo, producto escalar, producto por escalar...
      //      Si necesitas hacer que sea posible: escalar * vector.
      //      Defínelo así:
      //      O defínelo fuera
      //
      //      friend Vector3 operator*(T scalar, Vector3 const& other) {
  };
}  // namespace Fluid

#endif  //__VECTOR3_HPP
