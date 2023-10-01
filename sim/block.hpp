// Representación de un bloque

#ifndef __SIM_BLOCK_HPP
#define __SIM_BLOCK_HPP

#include "particle.hpp"

#include <ostream>
#include <utility>
#include <vector>

namespace Fluid {

  class Block {
    public:
      Block(std::size_t estimated_amount) { particles_.reserve(estimated_amount); };

      // Usamos semántica de movimientos mejor, así no es necesario hacer la copia
      constexpr void add(Particle && particle) { particles_.emplace_back(particle); };

      void clear() { particles_.clear(); };

      constexpr Particle operator[](std::size_t i) const { return particles_[i]; }

      constexpr auto begin() const { return particles_.begin(); }

      constexpr auto end() const { return particles_.end(); }

      constexpr auto begin() { return particles_.begin(); }

      constexpr auto end() { return particles_.end(); }

      friend std::ostream & operator<<(std::ostream & out, Block const & block) {
        out << "Block[";
        for (auto p : block) { out << p; }
        return out << "]";
      }

    private:
      std::vector<Particle> particles_{};
  };

}  // namespace Fluid
#endif  //__SIM_PROGARGS_HPP
