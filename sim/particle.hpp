#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "constants.hpp"
#include "vector3.hpp"

namespace Fluid {

  constexpr double density_factor = 315.0 / (64.0 * std::numbers::pi);

  struct Particle {
      // Parámetros que no cambian
      int id{};

      // Parámetros que cambian en cada iteración.
      // Es preferible en este caso nombres más cortos para que sea más fácil
      // escribir las fórmulas.
      Vector3<double> p{0, 0, 0};       // Posición
      double density{0};                // Densidad
      Vector3<double> hv{0, 0, 0};      // Gradiente de la velocidad
      Vector3<double> v{0, 0, 0};       // Velocidad
      Vector3<double> a{Constants::g};  // Aceleración

      Particle() = default;

      /// Construye una nueva partícula
      /// @param data
      /// Tupla (posición, gradiente, velocidad)
      /// @param id
      /// ID de la partícula
      /// @param density
      /// densidad de la partícula
      constexpr Particle(Vector3<Vector3<float>> const & data, int id, double density)
        : id{id}, p{static_cast<Vector3<double>>(data.x)}, density{density},
          hv{static_cast<Vector3<double>>(data.y)}, v{static_cast<Vector3<double>>(data.z)} { }

      // Operator '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n:   " << c.id << "\n"
                   << "\tpos:  " << c.p << "\n"
                   << "\thvel: " << c.hv << "\n"
                   << "\tvel:  " << c.v << "\n"
                   << "\tacc:  " << c.a << "\n"
                   << "\tρ:    " << c.density << "\n";
      }
  };

}  // namespace Fluid
#endif  // PARTICLE_HPP
