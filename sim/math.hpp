#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <cmath>
#include "vector3.hpp"
#include "constants.hpp"

namespace Fluid::Math {
    
    constexpr double CalcularMasaPartícula(double ppm){
        return Fluid::Constants::rho*pow(ppm,3);
    }

    constexpr double CalcularLongitudSuavizado(double ppm){
        return Fluid::Constants::r / ppm;
    }

    Fluid::Vector3<double> CalcularTamañoBloque(Fluid::Vector3<double> vmax, Fluid::Vector3<double> vmin, double h){
        return Vector3<double>{(vmax.x - vmin.x)/h,(vmax.y - vmin.y)/h,(vmax.z - vmin.z)/h};
    }

    Fluid::Vector3<double> CalcularTamañoMalla(Fluid::Vector3<double> vmax, Fluid::Vector3<double> vmin, Fluid::Vector3<double> n_i){
        return Vector3<double>{(vmax.x - vmin.x)/n_i.x,(vmax.y - vmin.y)/n_i.y,(vmax.z - vmin.z)/n_i.z};
    }
    
}

#endif