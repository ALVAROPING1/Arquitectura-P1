// Representación de un bloque

#ifndef SIM_BLOCK_HPP
#define SIM_BLOCK_HPP

#include "io.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <iostream>
#include <ostream>
#include <tuple>
#include <utility>
#include <vector>

namespace Fluid {

  // Objeto representando un bloque en la malla
  class Block {
      // Uses struct of arrays to store the data so that individual attributes
      // can be accessed separately without having to load all of them into cache
      //
      // Position/density and velocity/acceleration attributes are combined to reduce
      // the amount of cache sets required as they are commonly accessed together

      // Type combining the position and density of a particle
      struct PositionDensity {
          Vector3<double> pos;
          double dens;

          PositionDensity(Vector3<double> pos, double dens) : pos{pos}, dens{dens} { }
      };

      // Type combining the velocity and acceleration of a particle
      struct VelocityAcceleration {
          Vector3<double> vel, acc;

          VelocityAcceleration(Vector3<double> vel, Vector3<double> acc) : vel{vel}, acc{acc} { }
      };

      // Attributes stored

      std::vector<PositionDensity> position_density_{};  // Vector de atributos (posición, densidad)
      std::vector<Vector3<double>> hv_{};                // Vector del gradiente de la velocidad
      std::vector<VelocityAcceleration>
          velocity_acceleration_{};  // Vector de atributos (velocidad, aceleración)
      std::vector<int> id_{};        // Vector de IDs

    public:
      /// Construye un nuevo bloque
      /// @param estimated_amount
      /// Número de partículas para las que reservar espacio inicialmente
      constexpr explicit Block(std::size_t estimated_amount) {
        position_density_.reserve(estimated_amount);
        hv_.reserve(estimated_amount);
        velocity_acceleration_.reserve(estimated_amount);
        id_.reserve(estimated_amount);
      };

      /// Añade una partícula al bloque
      void add(Particle const & particle) {
        position_density_.emplace_back(particle.p, particle.density);
        hv_.push_back(particle.hv);
        velocity_acceleration_.emplace_back(particle.v, particle.a);
        id_.push_back(particle.id);
      }

      /// Vacía el bloque sin modificar la cantidad de espacio reservado
      void clear() {
        position_density_.clear();
        hv_.clear();
        velocity_acceleration_.clear();
        id_.clear();
      };

      /// Devuelve la cantidad de partículas en el bloque
      [[nodiscard]] constexpr std::size_t size() const {
        // All attribute vectors are appended to/cleared at once, so they will
        // always have the same size and it's safe to get the size from any of them
        return id_.size();
      }

      // These 2 getter functions would be better written if they were split into
      // returning individual fields. While this should result in the same code
      // after the definitions are inlined, for some reason, the compiler generates
      // significantly slower code if we do that. Thus, it will have to stay like this

      /// Devuelve una referencia al par (posición, densidad) de la partícula indicada
      /// @param i
      /// Índice de la partícula dentro del bloque
      constexpr PositionDensity & pos_dens(std::size_t i) { return position_density_[i]; }

      /// Devuelve una referencia al par (velocidad, aceleración) de la partícula indicada
      /// @param i
      /// Índice de la partícula dentro del bloque
      constexpr VelocityAcceleration & vel_acc(std::size_t i) { return velocity_acceleration_[i]; }

      /// Devuelve una referencia al gradiente de la velocidad de la partícula indicada
      /// @param i
      /// Índice de la partícula dentro del bloque
      constexpr Vector3<double> & hv(std::size_t i) { return hv_[i]; }

      /// Devuelve una copia de la partícula indicada
      /// @param i
      /// Índice de la partícula dentro del bloque
      constexpr Particle operator[](std::size_t i) const {
        Particle particle;
        particle.p       = position_density_[i].pos;
        particle.density = position_density_[i].dens;
        particle.v       = velocity_acceleration_[i].vel;
        particle.a       = velocity_acceleration_[i].acc;
        particle.hv      = hv_[i];
        particle.id      = id_[i];
        return particle;
      }

      /// Devuelve una copia de todas las partículas en el bloque
      [[nodiscard]] constexpr std::vector<Particle> get_particles() const {
        std::vector<Particle> out{};
        out.reserve(size());
        for (std::size_t i{0}; i < size(); ++i) { out.emplace_back(operator[](i)); }
        return out;
      }

      friend std::ostream & operator<<(std::ostream & out, Block const & block) {
        out << "Block[";
        for (std::size_t i{0}; i < block.size(); ++i) { out << block[i]; }
        return out << "]";
      }
  };

}  // namespace Fluid
#endif  // SIM_PROGARGS_HPP
