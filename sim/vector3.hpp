#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <ostream>

namespace Fluid {
    template<class T>
    struct Vector3 {
        T x{}, y{}, z{};

        Vector3(T c1, T c2, T c3) : x{c1}, y{c2}, z{c3} {};
        
        friend std::ostream & operator <<(std::ostream &out,Vector3 const &c){
            return out << "(" << "x: " << c.x << ",y: " << c.y << ",z: " << c.z << ")";
        }

        constexpr Vector3 operator+(const Vector3 &other) const {
            return Vector3{x+other.x,y+other.y,z+other.z};
        };
        constexpr Vector3 operator-(const Vector3 &other) const {
            return Vector3{x-other.x,y-other.y,z-other.z};
        };
    };
}

#endif
