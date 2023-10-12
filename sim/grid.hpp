// NOLINTBEGIN

// Representación de la malla
#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <iostream>
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
      template <bool Clamp>
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle const particle{{p, hv, v}, id_++, h_, m_};
        find_block<Clamp>(particle.p, front()).add(particle);
      }

      /// Ejecuta una iteración (paso) de la simulación.
      void step() {
        increment_densities();
        transfer_accelerations();
        wall_interactions();
        particle_movement();
        wall_collisions();
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
      constexpr void swap_buffers() {
        block_vector temp{std::move(blocks_.first)};
        blocks_.first  = std::move(blocks_.second);
        blocks_.second = std::move(temp);
      }
      [[nodiscard]] std::vector<Block> get_blocks() const { return blocks_.first; }

    private:
      [[nodiscard]] constexpr block_vector & front() { return blocks_.first; }

      [[nodiscard]] constexpr block_vector const & front() const { return blocks_.first; }

      [[nodiscard]] constexpr block_vector & back() { return blocks_.second; }

      [[nodiscard]] constexpr block_vector const & back() const { return blocks_.second; }

      // NOLINTNEXTLINE (readability-make-member-function-const)
      constexpr Block & index(Vector3<std::size_t> idx, block_vector & block_vec) {
        return block_vec[idx.z * count_.y * count_.x + idx.y * count_.x + idx.x];
      }

      /// Dada una posición devuelve el bloque donde se encuentra la partícula.
      /// @param pos
      /// La posición de la partícula
      template <bool Clamp>
      constexpr Block & find_block(Vector3<double> pos, block_vector & block_vec) {
        Vector3<std::size_t> idx{
            static_cast<Vector3<std::size_t>>((pos - Constants::b_min) / sizes_)};
        if (Clamp) { idx.clamp({0, 0, 0}, count_ - Vector3<std::size_t>{1, 1, 1}); }
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
          constexpr static void run(Particle & particle) {
            constexpr double epsilon = 1.0e-10;
            // NOLINTBEGIN (readability-identifier-length) Mathematical equation
            double const w  = Axis::get(particle.p) + Axis::get(particle.hv) * Constants::dt;
            double const dw = Constants::d_p - (Side == 0 ? w - Axis::get(Constants::b_min)
                                                          : Axis::get(Constants::b_max) - w);
            // NOLINTEND (readability-identifier-length)
            if (dw > epsilon) {
              Axis::set(particle.a) +=
                  (Side == 0 ? +1.0 : -1.0) *
                  (Constants::c_s * dw - Constants::d_v * Axis::get(particle.v));
            }
          }
      };

      struct collide_particle {
          template <int Side, typename Axis>
          constexpr static void run(Particle & particle) {
            double const d_w = (Side == 0 ? Axis::get(particle.p) - Axis::get(Constants::b_min)
                                          : Axis::get(Constants::b_max) - Axis::get(particle.p));
            if (d_w < 0) {
              Axis::set(particle.p)   = (Side == 0 ? Axis::get(Constants::b_min) - d_w
                                                   : Axis::get(Constants::b_max) + d_w);
              Axis::set(particle.v)  *= -1;
              Axis::set(particle.hv) *= -1;
            }
          }
      };

      template <int Side, typename A, typename B, typename C, typename Proc>
      constexpr void wall_iterate() {
        Vector3<std::size_t> idx{0, 0, 0};
        C::set(idx)               = (Side == 0 ? 0 : Side * C::get(count_) - 1);
        std::size_t const count_A = A::get(count_);
        std::size_t const count_B = B::get(count_);
        for (; A::get(idx) < count_A; ++A::set(idx)) {
          for (B::set(idx) = 0; B::get(idx) < count_B; ++B::set(idx)) {
            Block & block = index(idx, front());
            for (auto & particle : block) { Proc::template run<Side, C>(particle); }
          }
        }
      }

      template <typename Proc>
      constexpr void wall_action() {
        wall_iterate<0, Axis_X, Axis_Y, Axis_Z, Proc>();
        wall_iterate<1, Axis_X, Axis_Y, Axis_Z, Proc>();
        wall_iterate<0, Axis_X, Axis_Z, Axis_Y, Proc>();
        wall_iterate<1, Axis_X, Axis_Z, Axis_Y, Proc>();
        wall_iterate<0, Axis_Y, Axis_Z, Axis_X, Proc>();
        wall_iterate<1, Axis_Y, Axis_Z, Axis_X, Proc>();
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
        for (; idx.x < count_.x; ++idx.x) {
          for (idx.y = 0; idx.y < count_.y; ++idx.y) {
            for (idx.z = 0; idx.z < count_.z; ++idx.z) {
              f(idx);
            }
          }
        }
      }

      template <typename F>
      constexpr void iter_pairs(F f)
        requires std::invocable<F, Particle &, Particle &>
      {
        iter_blocks([this, f](Vector3<std::size_t> const block1) {
          adjacent_blocks(block1, [this, block1, f](const Vector3<std::size_t> block2) {
            for (auto & particle1 : index(block1, front())) {
              for (auto & particle2 : index(block2, front())) { f(particle1, particle2); }
            }
          });
          Block & block = index(block1, front());
          for (std::size_t i{}; i < block.size(); ++i) {
            for (std::size_t j{i + 1}; j < block.size(); ++j) { f(block[i], block[j]); }
          }
        });
      }

  };

}  // namespace Fluid

#endif  // GRID_HPP
// NOLINTEND
