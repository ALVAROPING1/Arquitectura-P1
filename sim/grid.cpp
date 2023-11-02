#include "grid.hpp"

#include "io.hpp"

#include <cmath>
#include <tuple>

namespace Fluid {

  std::vector<Particle> Grid::get_all() const {
    std::vector<Particle> result(pc_);  // Inicializa todas las Partículas
    for (auto const & block : front_) {
      for (std::size_t i{0}; i < block.size(); ++i) {
        auto const & particle{block[i]};
        result[particle.id] = particle;
      }
    }
    return result;
  }

  void Grid::increment_densities() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      Vector3<double> const distance{blockI.position(i) - blockJ.position(j)};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      density            = (density * density * density) * density_transform_factor_;
      blockI.density(i) += density;
      blockJ.density(j) += density;
    });
  }

  void Grid::transfer_accelerations() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      constexpr double const1{1e-12};
      constexpr double const2{15.0};
      constexpr double const3{45.0};
      Vector3<double> const distance{blockI.position(i) - blockJ.position(j)};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          // Clang-format doesn't know how to properly format this formula
          // clang-format off
          ( distance * (1.5 * Constants::p_s * (const2 / std::numbers::pi)) * (std::pow((h_ - dist_ij), 2) / dist_ij)
            * (blockI.density(i) + blockJ.density(j) - 2 * Constants::rho)
            + (blockJ.velocity(j) - blockI.velocity(i)) * Constants::mu * (const3/const2) * (const2 / std::numbers::pi))
           * acceleration_transform_factor_ / (blockI.density(i) * blockJ.density(j))
          // clang-format on
      };
      blockI.acceleration(i) += acceleration;
      blockJ.acceleration(j) -= acceleration;
    });
  }

  void Grid::particle_movement() {
    iter_blocks([this](Vector3<size_t> const & block_idx) {
      auto & block{index(block_idx, front_)};
      for (std::size_t i{0}; i < block.size(); ++i) {
        block.position(i) +=
            block.hv(i) * Constants::dt + block.acceleration(i) * std::pow(Constants::dt, 2);
        block.velocity(i)  = block.hv(i) + block.acceleration(i) * Constants::dt / 2;
        block.hv(i)       += block.acceleration(i) * Constants::dt;
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        block.density(i)      = reset_density_;
        block.acceleration(i) = Constants::g;
      }
    });
  }

  void Grid::particle_repositioning() {
    // TODO: skip clear loop on the first iteration
    for (auto & block : back_) { block.clear(); }
    for (auto & block : front_) {
      for (std::size_t i{0}; i < block.size(); ++i) {
        auto particle{block[i]};
        find_block(particle.p, back_).add(particle);
      }
    }
  }
}  // namespace Fluid
