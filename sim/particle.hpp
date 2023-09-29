#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vector3.hpp"
#include "constants.hpp"

namespace Fluid {
    class Particle {
    public:
        
        // Static parameters of the simulation.
        int id = 0;
        double density = 0;
        double mass = 0;

        // Varible parameters for each iteration.
        Vector3<double> position = Vector3<double> {0,0,0};        
        Vector3<double> hv = Vector3<double> {0,0,0};
        Vector3<double> velocity = Vector3<double> {0,0,0};
        
        // Class constructors.
        Particle() = default;
        Particle(double x, double y, double z) : position{x,y,z} {};
        
        //Operator '<<' for class printing.
        friend std::ostream& operator<<(std::ostream &out, Particle const &c){
            return out << "Particle n: " << c.id << "\n\tPosition: " << c.position << "\n\tVelocity: " << c.velocity << "\n"; 
        }

    };
    
    
}

#endif