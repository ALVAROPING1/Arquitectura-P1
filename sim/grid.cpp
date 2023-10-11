#include "grid.hpp"

namespace Fluid {

  std::vector<Particle> Grid::get_all () const {
    std::vector<Particle> result(pc_);    // Inicializa todas las Partículas
    for (auto const& block : front()) {
      for (auto const& particle : block) {
        result[particle.id] = particle;
      }
    }
    return result;
  }

}  // namespace Fluid
