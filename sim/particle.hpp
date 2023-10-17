#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "constants.hpp"
#include "vector3.hpp"

namespace Fluid {

  constexpr int ALIGN_VALUE       = 128;
  constexpr double density_factor = 315.0 / (64.0 * std::numbers::pi);

  // FIXME: Pensar más detenidamente el valor de alineamiento.
  //        Se podría separar a otro struct
  //
  // No toques esto.
  struct alignas(ALIGN_VALUE) Particle {
      // Parámetros que no cambian
      int id{};

      // Parámetros que cambian en cada iteración.
      // Es preferible en este caso nombres más cortos para que sea más fácil
      // escribir las fórmulas.
      Vector3<double> p{0, 0, 0};
      double density{0};
      Vector3<double> hv{0, 0, 0};
      Vector3<double> v{0, 0, 0};
      Vector3<double> a{Constants::g};

      Particle() = default;

      // Poner como constante de la clase 1/h³ * m

      constexpr Particle(Vector3<Vector3<float>> const & data, int id, double h, double m)
        : id{id}, p{static_cast<Vector3<double>>(data.x)}, density{reset_density(h, m)},
          hv{static_cast<Vector3<double>>(data.y)}, v{static_cast<Vector3<double>>(data.z)} {
      }

      constexpr static double reset_density(double h, double m) {
        return density_factor / (h * h * h) * m;
      }

      // Operator '<<' para imprimar la partícula (depurado).
      friend std::ostream & operator<<(std::ostream & out, Particle const & c) {
        return out << "Particle n:   " << c.id << "\n"
                   << "\tpos:  " << c.p << "\n"
                   << "\thvel: " << c.hv << "\n"
                   << "\tvel:  " << c.v << "\n"
                   << "\tacc:  " << c.a << "\n"
                   << "\tρ:    " << c.density << "\n";
      }

      // TODO: Añade funciones para modificar el estado de la partícula.
      //       De momento añade los parámetros que te parezcan. Yo luego
      //       buscaré cuáles son los mejores parámetros con más calma.
  };

}  // namespace Fluid
#endif  // PARTICLE_HPP
