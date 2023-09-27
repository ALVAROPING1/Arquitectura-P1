#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vector3.hpp"
#include "constants.hpp"

namespace Fluid {
    class Particle {
    public:
        int id = 0;
        Vector3 position = Vector3{0,0,0};        
        Vector3 acceleration = Vector3{0,9.8,0};
        double density = 0;

        Particle() {};
        Particle(double x, double y, double z) : position.x{x}, position.y{y}, position.z{z} {};
        
        friend std::ostream& operator<<(std::ostream &out, Particle){
            return out << "Partícula n: " << id << "\nPosition: " << position << "\nAcceleration: " << acceleration; 
        }

    };
    
    
}

#endif