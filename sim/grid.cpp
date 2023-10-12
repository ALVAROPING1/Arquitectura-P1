#include "grid.hpp"

#include <cmath>

namespace Fluid {

  std::vector<Particle> Grid::get_all() const {
    std::vector<Particle> result(pc_);  // Inicializa todas las Partículas
    for (auto const & block : front()) {
      for (auto const & particle : block) { result[particle.id] = particle; }
    }
    return result;
  }

  void Grid::increment_densities() {
    iter_pairs([this](Particle & particle1, Particle & particle2) {
      constexpr int const1{315};
      constexpr int const2{64};
      constexpr int const3{6};
      constexpr int const4{9};

      Vector3<double> const distance{particle1.p - particle2.p};
      double const norm{distance.square_norm()};
      if (norm >= std::pow(h_, 2)) { return; }
      double density{std::pow(h_, 2) - norm};
      density = (density * density * density + std::pow(h_, const3)) * const1 /
                (const2 * std::numbers::pi * std::pow(h_, const4));
      particle1.density += density;
      particle2.density += density;
    });
  }

  void Grid::transfer_accelerations() {
    iter_pairs([this](Particle & particle1, Particle & particle2) {
      constexpr double const1{1e-10};
      constexpr int const2{15};
      constexpr double const3{2.0};
      constexpr int const4{6};

      Vector3<double> const distance{particle1.p - particle2.p};
      double const norm{distance.square_norm()};
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          // Trash format, but that's the formula we were
          // given without meaning of individual parts
          (distance * (3 * Constants::p_s) / const3 * std::pow(h_ - dist_ij, 2) / dist_ij *
               (particle1.density + particle2.density - 2 * Constants::rho) +
           (particle2.v - particle1.v) * 3 * Constants::mu) *
          const2 / (std::numbers::pi * std::pow(h_, const4)) * m_ /
          (particle1.density * particle2.density)};
      particle1.a += acceleration;
      particle2.a -= acceleration;
    });
  }

  void Grid::particle_movement() {
    iter_blocks([this](Vector3<size_t> const block) {
      for (auto particle : index(block, front())) {
        particle.p  += particle.hv * Constants::dt + particle.a * std::pow(Constants::dt, 2);
        particle.v   = particle.hv + particle.a * Constants::dt / 2;
        particle.hv += particle.a * Constants::dt;
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        particle.density = 0;
        particle.a       = Constants::g;
      }
    });
  }

}  // namespace Fluid
