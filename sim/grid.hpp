// Representación de la malla
#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <iostream>
#include <utility>
#include <vector>
#include <tuple>

namespace Fluid {

  class Grid {
      using block_vector = std::vector<Block>;

      std::size_t pc_;
      double ppm_;
      double h_;
      double m_;
      int id_{};

      Vector3<std::size_t> count_;
      Vector3<double> sizes_;

      block_vector front_;
      block_vector back_;

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
        front_.resize(count, Block{approx});
        back_.resize(count, Block{approx});
      }

      /// Añade una partícula a la malla.
      /// @param p
      /// La posición de la partícula
      /// @param hv
      /// El parámetro hv de la partícula
      /// @param v
      /// La velocidad de la partícula
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle const particle{
            {p, hv, v},
            id_++, h_, m_
        };
        find_block(particle.p, front_).add(particle);
      }

      /// Ejecuta una iteración (paso) de la simulación.
      void step() {
        increment_densities();
        transfer_accelerations();
        wall_interactions();
        particle_movement();
        wall_collisions();
        // TODO: skip on the last iteration
        particle_repositioning();
        swap_buffers();
      }

      [[nodiscard]] std::vector<Particle> get_all() const;

#ifdef FLUID_DEBUG

    public:
#else

    private:
#endif
      void particle_repositioning();
      void increment_densities();
      void transfer_accelerations();
      void particle_movement();

      constexpr void wall_interactions() { wall_action<interact_particle>(); }

      constexpr void wall_collisions() { wall_action<collide_particle>(); }

      constexpr void swap_buffers() { std::swap(front_, back_); }

      [[nodiscard]] std::vector<Block> get_blocks() const { return front_; }

      void set_blocks(std::vector<Block> const & blocks) { front_ = blocks; }

    private:
      // The function can't be made const because it returns a reference to a stored value
      // NOLINTNEXTLINE (readability-make-member-function-const)
      constexpr Block & index(Vector3<std::size_t> idx, block_vector & block_vec) {
        return block_vec[idx.z * count_.y * count_.x + idx.y * count_.x + idx.x];
      }

      /// Dada una posición devuelve el bloque donde se encuentra la partícula.
      /// @param pos
      /// La posición de la partícula
      constexpr Block & find_block(Vector3<double> pos, block_vector & block_vec) {
        Vector3<double> const pos_{pos.clamp(Constants::b_min, Constants::b_max - sizes_)};
        Vector3<std::size_t> const idx{
            static_cast<Vector3<std::size_t>>((pos_ - Constants::b_min) / sizes_)};
        return index(idx, block_vec);
      }

      struct Axis_X {
          template <typename T>
          constexpr static T & set(Vector3<T> & v) {
            return v.x;
          }

          template <typename T>
          constexpr static T const & get(Vector3<T> const & v) {
            return v.x;
          }
      };

      struct Axis_Y {
          template <typename T>
          constexpr static T & set(Vector3<T> & v) {
            return v.y;
          }

          template <typename T>
          constexpr static T const & get(Vector3<T> const & v) {
            return v.y;
          }
      };

      struct Axis_Z {
          template <typename T>
          constexpr static T & set(Vector3<T> & v) {
            return v.z;
          }

          template <typename T>
          constexpr static T const & get(Vector3<T> const & v) {
            return v.z;
          }
      };

      struct interact_particle {
          template <int Side, typename Axis>
          constexpr static void run(Block & block, std::size_t i) {
            constexpr double epsilon = 1.0e-10;
            // NOLINTBEGIN (readability-identifier-length) Mathematical equation
            double const w  = Axis::get(block.position_density_[i].first) + Axis::get(block.hv_[i]) * Constants::dt;
            double const dw = Constants::d_p - (Side == 0 ? w - Axis::get(Constants::b_min)
                                                          : Axis::get(Constants::b_max) - w);
            // NOLINTEND (readability-identifier-length)
            if (dw > epsilon) {
              Axis::set(block.velocity_acceleration_[i].second) += (Side == 0 ? +1.0 : -1.0) * Constants::c_s * dw -
                                Constants::d_v * Axis::get(block.velocity_acceleration_[i].first);
            }
          }
      };

      struct collide_particle {
          template <int Side, typename Axis>
          constexpr static void run(Block & block, std::size_t i) {
            double const d_w = (Side == 0 ? Axis::get(block.position_density_[i].first) - Axis::get(Constants::b_min)
                                          : Axis::get(Constants::b_max) - Axis::get(block.position_density_[i].first));
            if (d_w < 0) {
              Axis::set(block.position_density_[i].first)   = (Side == 0 ? Axis::get(Constants::b_min) - d_w
                                                   : Axis::get(Constants::b_max) + d_w);
              Axis::set(block.velocity_acceleration_[i].first) *= -1;
              Axis::set(block.hv_[i])  *= -1;
            }
          }
      };

      template <int Side, typename A, typename B, typename C, typename Proc>
      constexpr void wall_iterate() {
        Vector3<std::size_t> idx{0, 0, 0};
        C::set(idx)               = (Side == 0 ? 0 : C::get(count_) - 1);
        std::size_t const count_A = A::get(count_);
        std::size_t const count_B = B::get(count_);
        for (; A::get(idx) < count_A; ++A::set(idx)) {
          for (B::set(idx) = 0; B::get(idx) < count_B; ++B::set(idx)) {
            Block & block = index(idx, front_);
            for (std::size_t i{0}; i < block.size(); ++i) { Proc::template run<Side, C>(block, i); }
          }
        }
      }

      template <typename Proc>
      constexpr void wall_action() {
        wall_iterate<0, Axis_Y, Axis_X, Axis_Z, Proc>();
        wall_iterate<1, Axis_Y, Axis_X, Axis_Z, Proc>();
        wall_iterate<0, Axis_Z, Axis_X, Axis_Y, Proc>();
        wall_iterate<1, Axis_Z, Axis_X, Axis_Y, Proc>();
        wall_iterate<0, Axis_Z, Axis_Y, Axis_X, Proc>();
        wall_iterate<1, Axis_Z, Axis_Y, Axis_X, Proc>();
      }

      template <typename F>
      void adjacent_blocks(Vector3<size_t> block, F f)
        requires std::invocable<F, Vector3<std::size_t> const>
      {
        if (block.x > 0) {
          if (block.y > 0) {
            if (block.z > 0) { f({block.x - 1, block.y - 1, block.z - 1}); }
            f({block.x - 1, block.y - 1, block.z});
            if (block.z < count_.z - 1) { f({block.x - 1, block.y - 1, block.z + 1}); }
          }
          if (block.z > 0) { f({block.x - 1, block.y, block.z - 1}); }
          f({block.x - 1, block.y, block.z});
          if (block.z < count_.z - 1) { f({block.x - 1, block.y, block.z + 1}); }
          if (block.y < count_.y - 1) {
            if (block.z > 0) { f({block.x - 1, block.y + 1, block.z - 1}); }
            f({block.x - 1, block.y + 1, block.z});
            if (block.z < count_.z - 1) { f({block.x - 1, block.y + 1, block.z + 1}); }
          }
        }
        if (block.y > 0) {
          if (block.z > 0) { f({block.x, block.y - 1, block.z - 1}); }
          f({block.x, block.y - 1, block.z});
          if (block.z < count_.z - 1) { f({block.x, block.y - 1, block.z + 1}); }
        }
        if (block.z > 0) { f({block.x, block.y, block.z - 1}); }
      }

      template <typename F>
      constexpr void iter_blocks(F f)
        requires std::invocable<F, Vector3<std::size_t> const>
      {
        Vector3<std::size_t> idx{0, 0, 0};
        for (; idx.z < count_.z; ++idx.z) {
          for (idx.y = 0; idx.y < count_.y; ++idx.y) {
            for (idx.x = 0; idx.x < count_.x; ++idx.x) { f(idx); }
          }
        }
      }

    public:
      template <typename F>
      constexpr void iter_pairs(F f)
        requires std::invocable<F, Block &, Block &, std::size_t, std::size_t>
      {
        iter_blocks([&](Vector3<std::size_t> const blockI_idx) {
          Block & blockI = index(blockI_idx, front_);
          adjacent_blocks(blockI_idx, [&](const Vector3<std::size_t> blockJ_idx) {
            Block & blockJ = index(blockJ_idx, front_);
            for (std::size_t i{0}; i < blockI.size(); ++i) {
              for (std::size_t j{0}; j < blockJ.size(); ++j) { f(blockI, blockJ, i, j); }
            }
          });
          for (std::size_t i{}; i < blockI.size(); ++i) {
            for (std::size_t j{i + 1}; j < blockI.size(); ++j) {  f(blockI, blockI, i, j);  }
          }
        });
      }
  };

}  // namespace Fluid

#endif  // GRID_HPP
