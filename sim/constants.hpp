#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "vector3.hpp"

namespace Fluid::Constants {
  // Constantes escalares de la simulación.
  // NOLINTBEGIN (readability-identifier-length) Mathematical constants are clearer when using their
  // standard names
  constexpr double r{1.695};     // Multiplicador de radio
  constexpr double rho{10e3};    // Densidad de fluido
  constexpr double p_s{3.0};     // Presión de rigidez
  constexpr double c_s{3.0e4};   // Colisiones de rigidez
  constexpr double d_v{128.0};   // Amortiguamiento
  constexpr double mu{0.4};      // Viscosidad
  constexpr double d_p{2.0e-4};  // Tamaño de partícula
  constexpr double dt{10e-3};    // Paso de tiempo

  // Constantes vectoriales de la simulación.
  constexpr Vector3<double> g{0.0, 9.8, 0.0};              // Aceleración externa
  constexpr Vector3<double> b_max{0.065, 0.1, 0.065};      // Límite superior de recinto
  constexpr Vector3<double> b_min{-0.065, -0.08, -0.065};  // Límite inferior de recinto
  // NOLINTEND (readability-identifier-length)
}  // namespace Fluid::Constants

#endif  // CONSTANTS_HPP
