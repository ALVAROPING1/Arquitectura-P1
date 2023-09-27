#ifndef VECTOR3_HPP
#define VECTOR3_HPP

namespace Fluid {
    template<class T>
    class Vector3 {
    public:
        T x = 0;
        T y = 0;
        T z = 0;

        Vector(T c1, T c2, T c3) : x{c1}, y{c2}, z{c3} {};
        
        friend std::ostream & operator <<(std::ofstream &out,Vector){
            return out << "(" << "x: " << x << ",y: " << y << ",z: " << z << ")";
        }

        inline Vector operator+(const Vector &other) const {return Vector{x+other.x,y+other.y,z+other.z};};
        inline Vector operator-(const Vector &other) const {return Vector{x-other.x,y-other.y,z-other.z};};
    };
}

#endif