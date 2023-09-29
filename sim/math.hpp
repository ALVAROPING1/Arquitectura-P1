#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <cmath>
#include <array>
#include "vector3.hpp"
#include "constants.hpp"

namespace Fluid::Math {
    
    constexpr double calc_particle_mass(double ppm){
        return Fluid::Constants::rho*pow(ppm,3);
    }

    template<class T>
    std::array<int,3> calc_particle_block_index(const std::array<T,3> &particle_pos, const std::array<T,3> &block_min_position, const std::array<T,3> &size_block_grid){
        std::array<int,3> indexes {
            static_cast<int> ( (particle_pos[0] - block_min_position[0])/size_block_grid[0] ),
            static_cast<int> ( (particle_pos[1] - block_min_position[1])/size_block_grid[1] ),
            static_cast<int> ( (particle_pos[2] - block_min_position[2])/size_block_grid[2] ),
        };

        return indexes;
    }

    constexpr double calc_h(double ppm){
        return Fluid::Constants::r / ppm;
    }

    Fluid::Vector3<double> calc_n_blocks(Fluid::Vector3<double> vmax, Fluid::Vector3<double> vmin, double h){
        return Vector3<double>{(vmax.x - vmin.x)/h,(vmax.y - vmin.y)/h,(vmax.z - vmin.z)/h};
    }

    Fluid::Vector3<double> calc_size_block_grid(Fluid::Vector3<double> vmax, Fluid::Vector3<double> vmin, Fluid::Vector3<double> n_bloques){
        return Vector3<double>{(vmax.x - vmin.x)/n_bloques.x,(vmax.y - vmin.y)/n_bloques.y,(vmax.z - vmin.z)/n_bloques.z};
    }


    
}

#endif