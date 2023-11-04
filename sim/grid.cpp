#include "grid.hpp"

#include "io.hpp"

#include <cmath>
#include <numbers>
#include <tuple>

namespace Fluid {

  std::vector<Particle> Grid::get_all() const {
    std::vector<Particle> result(amount_);  // Inicializa todas las Partículas
    // Itera por todas las partículas
    for (auto const & block : front_) {
      for (std::size_t i{0}; i < block.size(); ++i) {
        // Lee todos los atributos de la particula, y la almacena en su posición correspondiente
        // según el ID
        auto const particle{block[i]};
        result[particle.id] = particle;
      }
    }
    return result;
  }

  void Grid::increment_densities() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      Vector3<double> const distance{blockI.pos_dens(i).pos - blockJ.pos_dens(j).pos};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      density                  = (density * density * density) * density_transform_factor_;
      blockI.pos_dens(i).dens += density;
      blockJ.pos_dens(j).dens += density;
    });
  }

  void Grid::transfer_accelerations() {
    iter_pairs([&](Block & blockI, Block & blockJ, std::size_t i, std::size_t j) {
      constexpr double const1{1e-12};
      constexpr double const2{15.0};
      constexpr double const3{45.0};
      Vector3<double> const distance{blockI.pos_dens(i).pos - blockJ.pos_dens(j).pos};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          // Clang-format doesn't know how to properly format this formula
          // clang-format off
          ( distance * (1.5 * Constants::p_s * (const2 / std::numbers::pi)) * (std::pow((h_ - dist_ij), 2) / dist_ij)
            * (blockI.pos_dens(i).dens + blockJ.pos_dens(j).dens - 2 * Constants::rho)
            + (blockJ.vel_acc(j).vel - blockI.vel_acc(i).vel) * Constants::mu * (const3/const2) * (const2 / std::numbers::pi))
           * acceleration_transform_factor_ / (blockI.pos_dens(i).dens * blockJ.pos_dens(j).dens)
          // clang-format on
      };
      blockI.vel_acc(i).acc += acceleration;
      blockJ.vel_acc(j).acc -= acceleration;
    });
  }

  void Grid::particle_movement() {
    iter_blocks([this](Vector3<size_t> const & block_idx) {
      auto & block{index(block_idx, front_)};
      for (std::size_t i{0}; i < block.size(); ++i) {
        block.pos_dens(i).pos +=
            block.hv(i) * Constants::dt + block.vel_acc(i).acc * std::pow(Constants::dt, 2);
        block.vel_acc(i).vel  = block.hv(i) + block.vel_acc(i).acc * Constants::dt / 2;
        block.hv(i)          += block.vel_acc(i).acc * Constants::dt;
      }
    });
  }

  void Grid::particle_repositioning() {
    // TODO: skip clear loop on the first iteration
    // Vacía todos los bloques en el búfer de la siguiente iteración
    for (auto & block : back_) { block.clear(); }
    // Itera por todas las partículas en el búfer de la iteración actual
    for (auto & block : front_) {
      for (std::size_t i{0}; i < block.size(); ++i) {
        // Añade la partícula a su bloque correspondiente en el búfer de la siguiente iteración
        Particle particle;
        particle.p       = block.pos_dens(i).pos;
        particle.v       = block.vel_acc(i).vel;
        particle.hv      = block.hv(i);
        particle.id      = block.id(i);
        particle.density = reset_density_;
        find_block(particle.p, back_).add(particle);
      }
    }
  }
}  // namespace Fluid
