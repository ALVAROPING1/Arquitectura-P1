#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <algorithm>
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

      [[nodiscard]] constexpr T square_norm() const { return x * x + y * y + z * z; }

      constexpr void clamp(Vector3 const min, Vector3 const max) {
        x = std::clamp(x, min.x, max.x);
        y = std::clamp(y, min.y, max.y);
        z = std::clamp(z, min.z, max.z);
      }
  };
}  // namespace Fluid

#endif  // VECTOR3_HPP
