#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "constants.hpp"
#include "vector3.hpp"

#include <numbers>

namespace Fluid {

  constexpr double density_factor = 315.0 / (64.0 * std::numbers::pi);

  struct Particle {
      // Parámetros que no cambian
      int id{};

      // Parámetros que cambian en cada iteración.
      // Es preferible en este caso nombres más cortos para que sea más fácil
      // escribir las fórmulas.
      Vector3<double> pos;                // Posición
      double density{0};                  // Densidad
      Vector3<double> gradient;           // Gradiente de la velocidad
      Vector3<double> vel;                // Velocidad
      Vector3<double> acc{Constants::g};  // Aceleración

      Particle() = default;

      constexpr Particle(Vector3<float> const & pp, Vector3<float> const & phv,
                         Vector3<float> const & pv, int id)
        : id{id}, pos{static_cast<Vector3<double>>(pp)}, gradient{static_cast<Vector3<double>>(phv)},
          vel{static_cast<Vector3<double>>(pv)} { }

      // Operator '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n: "   << c.id
                   << "\n\tPosition: " << c.pos
                   << "\n\tVelocity: " << c.vel
                   << "\n\tAccelera: " << c.acc
                   << "\n\tGradient: " << c.gradient
                   << "\n\tDensity : " << c.density << std::endl;
      }
  };

}  // namespace Fluid
#endif  // PARTICLE_HPP
