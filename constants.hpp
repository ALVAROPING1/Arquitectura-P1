#ifndef __CONSTANTS_HPP
#define __CONSTANTS_HPP

namespace Fluid::Constants {
  constexpr float  r  = 1.695f;   // Multiplicador de radio
  constexpr float rho = 10e3f;    // Densidad de fluido
  constexpr float p_s = 3.0f;     // Presión de rigidez
  constexpr float s_c = 3.0e4f;   // Colisiones de rigidez
  constexpr float d_v = 128.0f;   // Amortiguamiento
  constexpr float mu  = 0.4f;     // Viscosidad
  constexpr float d_p = 2.0e-4f;  // Tamaño de partícula
  constexpr float dt  = 10e-3;    // Paso de tiempo
}

#endif//__CONSTANTS_HPP
