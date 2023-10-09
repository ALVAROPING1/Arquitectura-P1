// Representación de la malla
#ifndef __GRID_HPP
#define __GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <array>
#include <vector>

namespace Fluid {

  class Grid {
      using block_vector = std::vector<Block>;

      [[maybe_unused]] const std::size_t pc_;
      [[maybe_unused]] double const ppm_;
      [[maybe_unused]] double const h_;
      [[maybe_unused]] double const m_;
      int id_{};

      Vector3<std::size_t> const count_;
      Vector3<double> const sizes_;

      std::array<block_vector, 2> blocks_;
      block_vector & front_{blocks_[0]};
      block_vector & back_{blocks_[1]};

    public:
      /// Construye una nueva malla vacía.
      /// @param pc
      /// Número de partículas
      /// @param ppm
      /// Partículas por metro
      constexpr Grid(std::size_t pc, double ppm)
        : pc_{pc}, ppm_{ppm}, h_{Constants::r / ppm}, m_{Constants::rho / (ppm * ppm * ppm)},
          count_{static_cast<Vector3<std::size_t>>((Constants::b_max - Constants::b_min) / h_)},
          sizes_{(Constants::b_max - Constants::b_min) / static_cast<Vector3<double>>(count_)}
      {
        std::size_t const count = count_.x * count_.y * count_.z;
        std::size_t const approx = 2 * pc / count;
        blocks_[0].resize(count, Block{approx});
        blocks_[1].resize(count, Block{approx});
      }

      /// Añade una partícula a la malla.
      /// @param p
      /// La posición de la partícula
      /// @param hv
      /// El parámetro hv de la partícula
      /// @param v
      /// La velocidad de la partícula
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle P{p, hv, v, id_++};
        index(P.p).add(P);
      }

      /// Ejecuta una iteración (paso) de la simulación.
      void step() {
        // increment_densities();
        // transform_densities();
        // transfer_accelerations();
      }

      std::vector<Particle> get_all () const;

    private:
      constexpr void swap_buffers() { std::swap(front_, back_); }

    protected:
      /// Dada una posición devuelve el bloque donde se encuentra la partícula.
      /// @param pos
      /// La posición de la partícula
      constexpr Block & index(Vector3<double>) { return front_[0]; }
  };

}  // namespace Fluid

#endif  //__GRID_HPP
