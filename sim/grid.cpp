#include "grid.hpp"

#include "io.hpp"

#include <cmath>
#include <tuple>

namespace Fluid {

  std::vector<Particle> Grid::get_all() const {
    std::vector<Particle> result(pc_);  // Inicializa todas las Partículas
    for (auto const & block : front_) {
      for (std::size_t i{0}; i < block.size(); ++i) { 
        const auto & particle{block[i]};
        result[particle.id] = particle;
      }
    }
    return result;
  }

  void Grid::increment_densities() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      Vector3<double> const distance{blockI.position_density_[i].first - blockJ.position_density_[j].first};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      density            = (density * density * density) * density_transform_factor_;
      blockI.position_density_[i].second += density;
      blockJ.position_density_[j].second += density;
    });
  }

  // TODO: final implementation won't have the partigle_i and particle_j variables definitions.
  // Thus, it will be shorter and this will be fine
  // NOLINTNEXTLINE (readability-function-size)
  void Grid::transfer_accelerations() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      constexpr double const1{1e-12};
      constexpr double const2{15.0};
      constexpr double const3{45.0};
      Vector3<double> const distance{blockI.position_density_[i].first - blockJ.position_density_[j].first};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          // Clang-format doesn't know how to properly format this formula
          // clang-format off
          ( distance * (1.5 * Constants::p_s * (const2 / std::numbers::pi)) * (std::pow((h_ - dist_ij), 2) / dist_ij)
            * (blockI.position_density_[i].second + blockJ.position_density_[j].second - 2 * Constants::rho)
            + (blockJ.velocity_acceleration_[j].first - blockI.velocity_acceleration_[i].first) * Constants::mu * (const3/const2) * (const2 / std::numbers::pi))
           * acceleration_transform_factor_ / (blockI.position_density_[i].second * blockJ.position_density_[j].second)
          // clang-format on
      };
      blockI.velocity_acceleration_[i].second += acceleration;
      blockJ.velocity_acceleration_[j].second -= acceleration;
    });
  }

  // TODO: eliminar cuando eliminemos los comentarios con código de debug
  // NOLINTNEXTLINE (readability-function-size)
  void Grid::particle_movement() {
    iter_blocks([this](Vector3<size_t> const & block_idx) {
      auto & block{index(block_idx, front_)};
      for (std::size_t i{0}; i < block.size(); ++i) {
        // if (particle.id == 701) {
        //   IO::put_line(std::cerr, "Before movement:");
        //   IO::put_line(std::cerr, "Block idx: ", block);
        //   IO::put_line(std::cerr, "Correct Block idx: ",
        //                static_cast<Vector3<std::size_t>>(
        //                    (particle.p.clamp(Constants::b_min, Constants::b_max - sizes_) -
        //                     Constants::b_min) /
        //                    sizes_));
        //   IO::put_line(std::cerr, particle);
        // }
        // TODO: v/hv incrementan en valor absoluto en cada iteración. ¿Es esto correcto?
        block.position_density_[i].first += block.hv_[i] * Constants::dt + block.velocity_acceleration_[i].second * std::pow(Constants::dt, 2);
        block.velocity_acceleration_[i].first  = block.hv_[i] + block.velocity_acceleration_[i].second * Constants::dt / 2;
        block.hv_[i]  += block.velocity_acceleration_[i].second * Constants::dt;
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        block.position_density_[i].second = reset_density_;
        block.velocity_acceleration_[i].second = Constants::g;
        // if (particle.id == 701) {
        //   IO::put_line(std::cerr, "After movement:");
        //   IO::put_line(std::cerr, "Correct Block idx: ",
        //                static_cast<Vector3<std::size_t>>(
        //                    (particle.p.clamp(Constants::b_min, Constants::b_max - sizes_) -
        //                     Constants::b_min) /
        //                    sizes_));
        //   IO::put_line(std::cerr, particle);
        // }
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
