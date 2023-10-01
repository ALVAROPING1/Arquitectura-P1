#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "constants.hpp"
#include "vector3.hpp"

namespace Fluid {
  // FIXME: Pensar más detenidamente el valor de alineamiento.
  //        Se podría separar a otro struct
  //
  // No toques esto.
  struct alignas(128) Particle {
      // Parámetros que no cambian
      int id{};
      double density{};  // XXX: Densidad creo que no es necesario. Luego lo vemos
      double mass{};     // XXX: Masa creo que no es necesario. Luego lo vemos

      // Parámetros que cambian en cada iteración.
      // Es preferible en este caso nombres más cortos para que sea más fácil
      // escribir las fórmulas.
      Vector3<double> p  = Vector3<double>{0, 0, 0};
      Vector3<double> hv = Vector3<double>{0, 0, 0};
      Vector3<double> v  = Vector3<double>{0, 0, 0};

      // XXX: Implementa mejor este constructor, no cambies los parámetros
      constexpr Particle(Vector3<float> const & p, Vector3<float> const & hv,
                         Vector3<float> const & v, int id);

      // Operator '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n: " << c.id << "\n\tPosition: " << c.p
                   << "\n\tVelocity: " << c.v << "\n";
      }

      // TODO: Añade funciones para modificar el estado de la partícula.
      //       De momento añade los parámetros que te parezcan. Yo luego
      //       buscaré cuáles son los mejores parámetros con más calma.
  };

}  // namespace Fluid

#endif
