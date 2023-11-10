#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <algorithm>
#include <ostream>

namespace Fluid {
  enum class Axis : unsigned char { x, y, z };

  template <class T>
  struct Vector3 {
      T x{}, y{}, z{};

      friend std::ostream & operator<<(std::ostream & out, Vector3 const & c) {
        return out << "("
                   << "x: " << c.x << ",y: " << c.y << ",z: " << c.z << ")";
      }

      constexpr Vector3 operator+(Vector3 const & other) const {
        return Vector3{x + other.x, y + other.y, z + other.z};
      };

      constexpr Vector3 operator-(Vector3 const & other) const {
        return Vector3{x - other.x, y - other.y, z - other.z};
      };

      constexpr Vector3 operator+=(Vector3 const & other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
      };

      constexpr Vector3 operator-=(Vector3 const & other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
      };

      template <typename U>
      constexpr Vector3 operator*(U mul) const {
        return Vector3{x * mul, y * mul, z * mul};
      }

      template <typename U>
      constexpr Vector3 operator/(U div) const {
        return Vector3{x / div, y / div, z / div};
      }

      constexpr Vector3 operator/(Vector3 const & other) const {
        return Vector3{x / other.x, y / other.y, z / other.z};
      }

      template <typename U>
      constexpr explicit operator Vector3<U>() const {
        return Vector3<U>{static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
      }

      // Calcula la norma al cuadrado del vector
      [[nodiscard]] constexpr T square_norm() const { return x * x + y * y + z * z; }

      // Limita las coordenadas del vector al rango definido por los vectores
      /// @param min
      /// Vector con los valores mínimos de cada coordenada
      /// @param max
      /// Vector con los valores máximos de cada coordenada
      [[nodiscard]] constexpr Vector3 clamp(Vector3 const & min, Vector3 const & max) const {
        return Vector3{std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y),
                       std::clamp(z, min.z, max.z)};
      }

      constexpr bool operator<(Vector3 const & other) const {
        return (x < other.x || y < other.y || z < other.z);
      }

      constexpr bool operator>(Vector3 const& other) const {
        return (x > other.x || y > other.y || z > other.z); }

      constexpr bool operator== (Vector3 const& other) const {
        return (x == other.x && y == other.y && z == other.z); }

      template <Axis coordinate>
      [[nodiscard]] constexpr T const & get() const {
        return coordinate == Axis::x ? x : (coordinate == Axis::y ? y : z);
      }

      template <Axis coordinate>
      [[nodiscard]] constexpr T & set() {
        return coordinate == Axis::x ? x : (coordinate == Axis::y ? y : z);
      }
  };
}  // namespace Fluid

#endif  // VECTOR3_HPP
