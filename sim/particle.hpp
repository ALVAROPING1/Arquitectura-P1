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
      Vector3<double> pos;                      // Posición
      double density{0};                        // Densidad
      Vector3<double> gradient;                 // Gradiente de la velocidad
      Vector3<double> vel;                      // Velocidad
      Vector3<double> acc{Constants::gravity};  // Aceleración
      Particle() = default;

      /// Construye una nueva partícula
      /// @param data
      /// Tupla (posición, gradiente, velocidad)
      /// @param id
      /// ID de la partícula
      /// @param density
      /// densidad de la partícula
      constexpr Particle(Vector3<Vector3<float>> const & data, int id, double density)
        : id{id}, pos{static_cast<Vector3<double>>(data.x)}, density{density},
          gradient{static_cast<Vector3<double>>(data.y)},
          vel{static_cast<Vector3<double>>(data.z)} { }

      // Operador '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n: " << c.id << "\n\tPosition: " << c.pos
                   << "\n\tVelocity: " << c.vel << "\n\tAccelera: " << c.acc
                   << "\n\tGradient: " << c.gradient << "\n\tDensity : " << c.density << "\n";
      }

      constexpr bool operator==(Particle const & other) const {
        return pos == other.pos && density == other.density and gradient == other.gradient &&
               vel == other.vel && acc == other.acc && id == other.id;
      }
  };

}  // namespace Fluid

#endif  // PARTICLE_HPP
