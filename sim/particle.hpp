#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "constants.hpp"
#include "vector3.hpp"

namespace Fluid {

  constexpr int ALIGN_VALUE = 128;

  // FIXME: Pensar más detenidamente el valor de alineamiento.
  //        Se podría separar a otro struct
  //
  // No toques esto.
  struct alignas(ALIGN_VALUE) Particle {
      // Parámetros que no cambian
      int id{};
      double density{};

      // Parámetros que cambian en cada iteración.
      // Es preferible en este caso nombres más cortos para que sea más fácil
      // escribir las fórmulas.
      Vector3<double> p  = Vector3<double>{0, 0, 0};
      Vector3<double> hv = Vector3<double>{0, 0, 0};
      Vector3<double> v  = Vector3<double>{0, 0, 0};

      Particle() = default;

      constexpr Particle(Vector3<float> const & pp, Vector3<float> const & phv,
                         Vector3<float> const & pv, int id)
        : id{id}, p{static_cast<Vector3<double>>(pp)}, hv{static_cast<Vector3<double>>(phv)},
          v{static_cast<Vector3<double>>(pv)} { }

      // Operator '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n: " << c.id << "\n\tPosition: " << c.p << "\n\tVelocity: " << c.v
                   << "\n";
      }

      // TODO: Añade funciones para modificar el estado de la partícula.
      //       De momento añade los parámetros que te parezcan. Yo luego
      //       buscaré cuáles son los mejores parámetros con más calma.
  };

}  // namespace Fluid
#endif  // PARTICLE_HPP
