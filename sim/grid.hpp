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
        // particle_repositioning();
        increment_densities();
        transfer_accelerations();
        wall_interactions();
        particle_movement();
        wall_collisions();
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
        return block_vec[idx.x * count_.y * count_.z + idx.y * count_.z + idx.z];
      }

      /// Dada una posición devuelve el bloque donde se encuentra la partícula.
      /// @param pos
      /// La posición de la partícula
      constexpr Block & find_block(Vector3<double> pos, block_vector & block_vec) {
        return index(static_cast<Vector3<std::size_t>>((pos - Constants::b_min) / sizes_),
                     block_vec);
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
            double const w        = Axis::get(particle.p) + Axis::get(particle.hv) * Constants::dt;
            Axis::set(particle.p) = w;
            double const dw       = Constants::d_p - (Side == 0 ? w - Axis::get(Constants::b_min)
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

      std::vector<Vector3<std::size_t>> adjacent_blocks(Vector3<std::size_t> block);

      [[nodiscard]] std::vector<Vector3<std::size_t>> iter_blocks() const {
        std::vector<Vector3<std::size_t>> out{};
        out.reserve(count_.x * count_.y * count_.z);
        for (std::size_t i{0}; i < count_.x; ++i) {
          for (std::size_t j{0}; j < count_.y; ++j) {
            for (std::size_t k{0}; k < count_.z; ++k) { out.push_back({i, j, k}); }
          }
        }
        return out;
      }

      template <typename F>
      constexpr void iter_pairs(F f)
        requires std::invocable<F, Particle &, Particle &>
      {
        for (auto block1 : iter_blocks()) {
          for (auto block2 : adjacent_blocks(block1)) {
            for (auto & particle1 : index(block1, front())) {
              for (auto & particle2 : index(block2, front())) { f(particle1, particle2); }
            }
          }
          Block & block = index(block1, front());
          for (std::size_t i{}; i < block.size(); ++i) {
            for (std::size_t j{i + 1}; j < block.size(); ++j) { f(block[i], block[j]); }
          }
        }
      }

      void particle_repositioning();
      void _increment_densities(Particle & particle1, Particle & particle2);

      void increment_densities() {
        iter_pairs([this](Particle & particle1, Particle & particle2) {
          _increment_densities(particle1, particle2);
        });
      }

      void _transfer_accelerations(Particle & particle1, Particle & particle2);

      void transfer_accelerations() {
        iter_pairs([this](Particle & particle1, Particle & particle2) {
          _transfer_accelerations(particle1, particle2);
        });
      }

      void particle_movement();

      constexpr void wall_interactions() { wall_action<interact_particle>(); }

      constexpr void wall_collisions() { wall_action<collide_particle>(); }
  };

}  // namespace Fluid

#endif  // GRID_HPP
