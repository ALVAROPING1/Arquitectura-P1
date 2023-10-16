#include "grid.hpp"

#include "io.hpp"

#include <cmath>

namespace Fluid {

  std::vector<Particle> Grid::get_all() const {
    std::vector<Particle> result(pc_);  // Inicializa todas las Partículas
    for (auto const & block : front_) {
      for (auto const & particle : block) { result[particle.id] = particle; }
    }
    return result;
  }

  void Grid::increment_densities() {
    iter_pairs([this](Particle & particle1, Particle & particle2) {
      // constexpr int nine{9};

      Vector3<double> const distance{particle1.p - particle2.p};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      // density = (density * density * density) * density_factor / std::pow(h_, nine) * m_;
      density            = density * density * density;
      particle1.density += density;
      particle2.density += density;
    });
  }

  void Grid::transform_densities() {
    double const h_3    = h_ * h_ * h_;
    double const h_6    = h_3 * h_3;
    double const h_9    = h_6 * h_3;
    double const factor = density_factor / h_9 * m_;
    for (auto & block : front_) {
      for (auto & particle : block) { particle.density = (particle.density + h_6) * factor; }
    }
  }

  void Grid::transfer_accelerations() {
    iter_pairs([this](Particle & particle_i, Particle & particle_j) {
      constexpr double const1{1e-12};
      constexpr int const2{15};
      constexpr int const4{6};
      constexpr double const5{45.0};

      Vector3<double> const distance{particle_i.p - particle_j.p};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double const dist_ij{std::sqrt(std::max(norm, const1))};
      Vector3<double> const acceleration{
          ((particle_i.p - particle_j.p) * (const2 / (std::numbers::pi * std::pow(h_, const4))) *
               (1.5 * m_ * Constants::p_s) * (std::pow((h_ - dist_ij), 2) / dist_ij) *
               (particle_i.density + particle_j.density - 2 * Constants::rho) +
           (particle_j.v - particle_i.v) * (const5 / (std::numbers::pi * std::pow(h_, 6))) *
               Constants::mu * m_) /
          (particle_i.density * particle_j.density)};
      particle_i.a += acceleration;
      particle_j.a -= acceleration;
    });
  }

  void Grid::particle_movement() {
    // TODO: Make block variable
    iter_blocks([this](Vector3<size_t> const & block) {
      for (auto & particle : index(block, front_)) {
        // TODO: v/hv incrementan en valor absoluto en cada iteración. ¿Es esto correcto?
        particle.p  += particle.hv * Constants::dt + particle.a * std::pow(Constants::dt, 2);
        particle.v   = particle.hv + particle.a * Constants::dt / 2;
        particle.hv += particle.a * Constants::dt;
        // TODO: Reset here accelerations
      }
    });
  }

  void Grid::particle_repositioning() {
    // TODO: skip clear loop on the first iteration
    for (auto & block : back_) { block.clear(); }
    for (auto & block : front_) {
      for (auto & particle : block) {
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        particle.reset_density(h_, m_);
        particle.a = Constants::g;
        find_block(particle.p, back_).add(particle);
      }
    }
  }
}  // namespace Fluid
