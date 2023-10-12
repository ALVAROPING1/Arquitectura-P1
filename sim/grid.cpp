// NOLINTBEGIN
#include "io.hpp"
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
      constexpr int nine{9};

      Vector3<double> const distance{particle1.p - particle2.p};
      double const norm{distance.square_norm()};
      if (norm >= h_ * h_) { return; }
      double density{h_ * h_ - norm};
      density = (density * density * density) * density_factor / std::pow(h_, nine) * m_;
      particle1.density += density;
      particle2.density += density;
    });
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
      Vector3<double> const acceleration {
        (
          (particle_i.p - particle_j.p)
          * (const2 / (std::numbers::pi * std::pow(h_, const4)))
          * (1.5 * m_ * Constants::p_s)
          * (std::pow((h_ - dist_ij), 2) / dist_ij)
          * (particle_i.density + particle_j.density - 2 * Constants::rho)
          + (particle_j.v - particle_i.v)
          * (const5 / (std::numbers::pi * std::pow(h_, 6)))
          * Constants::mu * m_
        )
          /
        (particle_i.density * particle_j.density)
      };
      // Vector3<double> const acceleration{
      //     // Trash format, but that's the formula we were
      //     // given without meaning of individual parts
      //     (distance * (3 * Constants::p_s) / const3 * std::pow(h_ - dist_ij, 2) / dist_ij *
      //          (particle_i.density + particle_j.density - 2 * Constants::rho) +
      //      (particle_j.v - particle_i.v) * 3 * Constants::mu) *
      //     const2 / (std::numbers::pi * std::pow(h_, const4)) * m_ /
      //     (particle_i.density * particle_j.density)};
      particle_i.a += acceleration;
      particle_j.a -= acceleration;
    });
  }

  void Grid::particle_movement() {
    // TODO: Make block variable
    iter_blocks([this](Vector3<size_t> const& block) {
      for (auto & particle : index(block, front())) {
        particle.p  += particle.hv * Constants::dt + particle.a * std::pow(Constants::dt, 2);
        particle.v   = particle.hv + particle.a * Constants::dt / 2;
        particle.hv += particle.a * Constants::dt;
        // Reset the density/acceleration while we are at it as they are no longer needed in this
        // iteration. This allows us to skip the full scan of the particles at the beginning of each
        // update cycle
        particle.reset_density(h_, m_);
        particle.a       = Constants::g;
      }
    });
  }

  void Grid::particle_repositioning() {
    block_vector & back = blocks_.second;
    for (auto & block : back) { block.clear(); }
    blocks_.second.resize(front().size(), Block{10});
    int i = 0;
    for (auto const& block : front()) {
      IO::put_line (std::cerr, "Io", block, " ", ++i, "/", front().size());
      for (auto const& particle : block) {
        Vector3<std::size_t> idx{
            static_cast<Vector3<std::size_t>>((particle.p - Constants::b_min) / sizes_)};
        const std::size_t tot = idx.z * count_.y * count_.x + idx.y * count_.x + idx.x;
        IO::put_line(std::cerr, idx, " => ", tot, "/", back.size());
        // find_block<false>(particle.p, back()).add(particle);
        back[tot].add(particle);
        IO::put_line (std::cerr, "Fuck");
        back[tot].add(particle);
        IO::put_line (std::cerr, "Fuck");
      }
      IO::put_line (std::cerr, "Oi");
    }
  }
}  // namespace Fluid
// NOLINTEND
