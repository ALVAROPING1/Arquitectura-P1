#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vector3.hpp"
#include "constants.hpp"

namespace Fluid {
    class Particle {
    public:
        int id = 0;
        Vector3<double> position = Vector3<double> {0,0,0};        
        Vector3<double> acceleration = Vector3<double> {0,9.8,0};
        double density = 0;

        Particle() = default;
        Particle(double x, double y, double z) : position{x,y,z} {};
        
        friend std::ostream& operator<<(std::ostream &out, Particle const &c){
            return out << "Partícula n: " << c.id << "\nPosition: " << c.position << "\nAcceleration: " << c.acceleration; 
        }

    };
    
    
}

#endif