#ifndef __VECTOR3_HPP
#define __VECTOR3_HPP

#include <array>
#include <ostream>

namespace Fluid {
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

      template<typename U>
      constexpr Vector3 operator/(U div) const {
        return Vector3{x / div, y / div, z / div};
      }

      constexpr Vector3 operator/(Vector3 const & other) const {
        return Vector3{x / other.x, y / other.y, z / other.z};
      }

      template<typename U>
      constexpr explicit operator Vector3<U> () const {
        return Vector3<U>{static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
      }

  };
}  // namespace Fluid

#endif  //__VECTOR3_HPP
