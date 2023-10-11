// Representación de un bloque

#ifndef SIM_BLOCK_HPP
#define SIM_BLOCK_HPP

#include "particle.hpp"

#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

namespace Fluid {

  class Block {
      std::vector<Particle> particles_{};

    public:
      constexpr explicit Block(std::size_t estimated_amount) {
        particles_.reserve(estimated_amount);
      };

      // Usamos semántica de movimientos mejor, así no es necesario hacer la copia
      void add(Particle const & particle) { particles_.push_back(particle); }

      void clear() { particles_.clear(); };

      constexpr Particle & operator[](std::size_t i) { return particles_[i]; }

      constexpr Particle const & operator[](std::size_t i) const { return particles_[i]; }

      [[nodiscard]] constexpr std::size_t size() const { return particles_.size(); }

      [[nodiscard]] constexpr auto begin() const { return particles_.begin(); }

      [[nodiscard]] constexpr auto end() const { return particles_.end(); }

      constexpr auto begin() { return particles_.begin(); }

      constexpr auto end() { return particles_.end(); }

      friend std::ostream & operator<<(std::ostream & out, Block const & block) {
        out << "Block[";
        for (auto particle : block) { out << particle; }
        return out << "]";
      }
  };

}  // namespace Fluid
#endif  // SIM_PROGARGS_HPP
