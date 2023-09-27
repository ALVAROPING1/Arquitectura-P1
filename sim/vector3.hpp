#ifndef VECTOR3_HPP
#define VECTOR3_HPP

namespace Fluid {
    
    class Vector {
    private:
        double x = 0;
        double y = 0;
        double z = 0;
    public:
        inline Vector(double x, double y, double z) : this.x{x}, this.y{y}, this.z{z} {};
        
        void print() const{
            std::cout << "( x: " << x << ", y: " << y << ", z: " << z << ")" << std::endl;
        }

        inline Vector operator+(const Vector &other) const {return Vector(x+other.x,y+other.y,z+other.z);};
        inline Vector operator-(const Vector &other) const {return Vector(x-other.x,y-other.y,z-other.z);};
    };
}

#endif