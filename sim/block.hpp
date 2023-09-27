// Representación de un bloque

#ifndef __SIM_BLOCK_HPP
#define __SIM_BLOCK_HPP

#include <ostream>
#include <utility>
#include <vector>
#include "particle.hpp"

namespace Fluid {

  class Block {
    public:
      Block(std::size_t estimated_amount) {particles_.reserve(estimated_amount);} ;
      void add(Particle particle) {particles_.push_back(particle);};
      void clear() {particles_.clear();};
      Particle operator[](std::size_t i) const { return particles_[i]; }

      auto begin() const { return particles_.begin(); }
      auto end() const { return particles_.end(); }
      auto begin() { return particles_.begin(); }
      auto end() { return particles_.end(); }

      friend std::ostream& operator<<(std::ostream& out, Block const& block) {
        out << "Block[";
        for (auto p: block) {
          out << p;
        }
        return out << "]";
      }

    private:
      std::vector<Particle> particles_{};

  };

}
#endif//__SIM_PROGARGS_HPP
