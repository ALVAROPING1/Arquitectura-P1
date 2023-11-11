#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "vector3.hpp"

namespace Fluid::Constants {
  // Constantes escalares de la simulación.
  constexpr double radius{1.695};       // Multiplicador de radio
  constexpr double density{1.0e3};      // Densidad de fluido
  constexpr double pressure{3.0};       // Presión de rigidez
  constexpr double collisions{3.0e4};   // Colisiones de rigidez
  constexpr double damping{128.0};      // Amortiguamiento
  constexpr double viscosity{0.4};      // Viscosidad
  constexpr double size{2.0e-4};        // Tamaño de partícula
  constexpr double delta_time{1.0e-3};  // Paso de tiempo

  constexpr Vector3<double> gravity{0.0, -9.8, 0.0};       // Aceleración externa
  constexpr Vector3<double> b_max{0.065, 0.1, 0.065};      // Límite superior de recinto
  constexpr Vector3<double> b_min{-0.065, -0.08, -0.065};  // Límite inferior de recinto
}  // namespace Fluid::Constants

#endif  // CONSTANTS_HPP
