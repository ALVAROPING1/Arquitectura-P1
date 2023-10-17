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
    iter_pairs<Block::Position, Block::Density>([&](std::tuple<Block::Position::type, Block::Density::type> particle_i,
                                                    std::tuple<Block::Position::type, Block::Density::type> particle_j) {
      constexpr int nine{9};

      auto [pos_i, density_i]{particle_i};
      auto [pos_j, density_j]{particle_j};
      Vector3<double> const distance{pos_i - pos_j};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      density            = (density * density * density) * density_factor / std::pow(h_, nine) * m_;
      density_i += density;
      density_j += density;
    });
  }

  // TODO: final implementation won't have the partigle_i and particle_j variables definitions.
  // Thus, it will be shorter and this will be fine
  // NOLINTNEXTLINE (readability-function-size)
  void Grid::transfer_accelerations() {
    iter_pairs<Block::Position, Block::Velocity, Block::Acceleration, Block::Density>([&](std::tuple<Block::Position::type, Block::Velocity::type, Block::Acceleration::type, Block::Density::type> particle_it,
                                                                                          std::tuple<Block::Position::type, Block::Velocity::type, Block::Acceleration::type, Block::Density::type> particle_jt) {
      constexpr double const1{1e-12};
      constexpr double const2{15.0};
      constexpr double const3{45.0};
      auto [pos_i, vel_i, acc_i, density_i]{particle_it};
      auto [pos_j, vel_j, acc_j, density_j]{particle_jt};
      Vector3<double> const distance{pos_i - pos_j};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          // Clang-format doesn't know how to properly format this formula
          // clang-format off
          ( distance * (1.5 * Constants::p_s) * (std::pow((h_ - dist_ij), 2) / dist_ij)
            * (density_i + density_j - 2 * Constants::rho)
            + (vel_j - vel_i) * Constants::mu * (const3/const2))
          * (const2 / (std::numbers::pi * std::pow(h_, 6))) * m_ / (density_i * density_j)
          // clang-format on
      };
      acc_i += acceleration;
      acc_j -= acceleration;
    });
  }

  // TODO: eliminar cuando eliminemos los comentarios con código de debug
  // NOLINTNEXTLINE (readability-function-size)
  void Grid::particle_movement() {
    iter_blocks([this](Vector3<size_t> const & block_idx) {
      auto & block{index(block_idx, front_)};
      for (std::size_t i{0}; i < block.size(); ++i) {
        auto [pos, vel, hv, acc, density]{block.query<Block::Position, Block::Velocity, Block::Hv, Block::Acceleration, Block::Density>(i)};
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
        pos += hv * Constants::dt + acc * std::pow(Constants::dt, 2);
        vel  = hv + acc * Constants::dt / 2;
        hv  += acc * Constants::dt;
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        density = Particle::reset_density(h_, m_);
        acc = Constants::g;
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
