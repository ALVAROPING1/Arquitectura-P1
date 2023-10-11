// Representación de la malla
#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <tuple>
#include <utility>
#include <vector>

namespace Fluid {

  class Grid {
      using block_vector = std::vector<Block>;

      // TODO: solucionar errores de clang-tidy
      [[maybe_unused]] std::size_t pc_;
      [[maybe_unused]] double ppm_;
      [[maybe_unused]] double h_;
      [[maybe_unused]] double m_;
      int id_{};

      Vector3<std::size_t> count_;
      Vector3<double> sizes_;

      std::pair<block_vector, block_vector> blocks_;

    public:
      /// Construye una nueva malla vacía.
      /// @param pc
      /// Número de partículas
      /// @param ppm
      /// Partículas por metro
      constexpr Grid(std::size_t pc, double ppm)
        : pc_{pc}, ppm_{ppm}, h_{Constants::r / ppm}, m_{Constants::rho / (ppm * ppm * ppm)},
          count_{static_cast<Vector3<std::size_t>>((Constants::b_max - Constants::b_min) / h_)},
          sizes_{(Constants::b_max - Constants::b_min) / static_cast<Vector3<double>>(count_)} {
        std::size_t const count  = count_.x * count_.y * count_.z;
        std::size_t const approx = 2 * pc / count;
        blocks_.first.resize(count, Block{approx});
        blocks_.second.resize(count, Block{approx});
      }

      /// Añade una partícula a la malla.
      /// @param p
      /// La posición de la partícula
      /// @param hv
      /// El parámetro hv de la partícula
      /// @param v
      /// La velocidad de la partícula
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle const particle{p, hv, v, id_++};
        find_block(particle.p, front()).add(particle);
      }

      /// Ejecuta una iteración (paso) de la simulación.
      void step() {
        // increment_densities();
        // transfer_accelerations();
        // wall_interactions();
        // particle_movement();
        // wall_collisions();
        swap_buffers();
      }

      [[nodiscard]] std::vector<Particle> get_all() const;

    private:
      constexpr void swap_buffers() {
        block_vector temp{std::move(blocks_.first)};
        blocks_.first  = std::move(blocks_.second);
        blocks_.second = std::move(temp);
      }

      [[nodiscard]] constexpr block_vector & front() { return blocks_.first; }

      [[nodiscard]] constexpr block_vector const & front() const { return blocks_.first; }

      [[nodiscard]] constexpr block_vector & back() { return blocks_.second; }

      [[nodiscard]] constexpr block_vector const & back() const { return blocks_.second; }

      // NOLINTNEXTLINE (readability-make-member-function-const)
      constexpr Block & index(Vector3<std::size_t> idx, block_vector & block_vec) {
        return block_vec[idx.y * count_.x * count_.z + idx.x * count_.z + idx.z];
      }

      /// Dada una posición devuelve el bloque donde se encuentra la partícula.
      /// @param pos
      /// La posición de la partícula
      constexpr Block & find_block(Vector3<double> pos, block_vector & block_vec) {
        return index(static_cast<Vector3<std::size_t>>((pos - Constants::b_min) / sizes_),
                     block_vec);
      }
  };

}  // namespace Fluid

#endif  // GRID_HPP
