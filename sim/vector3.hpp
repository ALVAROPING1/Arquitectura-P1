#ifndef VECTOR3_HPP
#define VECTOR3_HPP

namespace Fluid {
    
    class Vector {
    public:
        double x = 0;
        double y = 0;
        double z = 0;

        Vector(double c1, double c2, double c3) : x{c1}, y{c2}, z{c3} {};
        
        friend std::ostream & operator <<(std::ofstream &out,Vector){
            return out << "(" << "x: " << x << ",y: " << y << ",z: " << z << ")";
        }

        inline Vector operator+(const Vector &other) const {return Vector{x+other.x,y+other.y,z+other.z};};
        inline Vector operator-(const Vector &other) const {return Vector{x-other.x,y-other.y,z-other.z};};
    };
}

#endif