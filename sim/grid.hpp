// Representación de la malla
#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <cmath>
#include <utility>
#include <vector>
#include <numbers>

namespace Fluid {

  // Objeto representando la malla de la simulación
  class Grid {

#ifdef FLUID_DEBUG

    public:
#else

    private:
#endif

      using block_vector = std::vector<Block>;

      double h_;                              // Longitud de suavizado
      double m_;                              // Masa de las partículas
      double density_transform_factor_;       // Factor de transformación de la densidad
      double acceleration_transform_factor_;  // Factor de transformación de la aceleración
      double reset_density_;                  // Densidad inicial de las partículas
      int amount_{};                          // Número de partículas

      Vector3<std::size_t> count_;  // Cantidad de bloques en cada eje
      Vector3<double> sizes_;       // Tamaño de los bloques en cada eje

      block_vector front_;  // Búfer de bloques en la iteración actual
      block_vector back_;   // Búfer de bloques en la siguiente iteración

      friend class GridTestHarness;

    public:
      /// Construye una nueva malla vacía.
      /// @param pc
      /// Número de partículas
      /// @param ppm
      /// Partículas por metro
      Grid(std::size_t pc, double ppm)
        : h_{Constants::r / ppm}, m_{Constants::rho / (ppm * ppm * ppm)},
          density_transform_factor_{density_factor / std::pow(h_, 3 * 3) * m_},
          acceleration_transform_factor_{m_ / std::pow(h_, 2 * 3)},
          reset_density_{density_factor / std::pow(h_, 3) * m_},
          count_{static_cast<Vector3<std::size_t>>((Constants::b_max - Constants::b_min) / h_)},
          sizes_{(Constants::b_max - Constants::b_min) / static_cast<Vector3<double>>(count_)} {
        std::size_t const count  = count_.x * count_.y * count_.z;
        std::size_t const approx = pc / count;
        front_.resize(count, Block{approx});
        back_.resize(count, Block{approx});
      }

      /// Añade una partícula a la malla.
      /// @param p
      /// La posición de la partícula
      /// @param hv
      /// El gradiente de la velocidad de la partícula
      /// @param v
      /// La velocidad de la partícula
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle const particle{
            {p, hv, v},
            amount_++, reset_density_
        };
        find_block(particle.pos, front_).add(particle);
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

      /// Devuelve todas las partículas de la malla
      [[nodiscard]] std::vector<Particle> get_all() const {
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

    private:
      /// Indexa el vector de bloques según las coordendas de bloque dadas
      /// @param idx
      /// Índice de bloque
      /// @param block_vec
      /// Vector de bloques a indexar. Debería ser `front_` o `back_`
      //
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

      /// Calcula la interacción de una partícula con el borde dado
      struct interact_particle {
          /// Ejecuta el cálculo
          /// @tparam Side
          /// Borde inferior (true) o superior (false)
          /// @tparam Axis
          /// Coordenada en la que aplicar la interacción
          /// @param block
          /// Bloque en el que está la partícula
          /// @param i
          /// Índice de la partícula en el bloque
          template <bool Side, Axis W>
          constexpr static void run(Block & block, std::size_t i) {
            constexpr double epsilon = 1.0e-10;
            // NOLINTBEGIN (readability-identifier-length) Mathematical equation
            double const w  = block.pos_dens(i).pos.get<W>() + block.hv(i).get<W>() * Constants::dt;
            double const dw = Constants::d_p - (Side ? w - Constants::b_min.get<W>()
                                                     : Constants::b_max.get<W>() - w);
            // NOLINTEND (readability-identifier-length)
            if (dw > epsilon) {
              block.vel_acc(i).acc.set<W>() += (Side ? +1.0 : -1.0) * Constants::c_s * dw -
                                               Constants::d_v * block.vel_acc(i).vel.get<W>();
            }
          }
      };

      /// Calcula la colisión de una partícula con el borde dado
      struct collide_particle {
          /// Ejecuta el cálculo
          /// @tparam Side
          /// Borde inferior (true) o superior (false)
          /// @tparam Axis
          /// Coordenada en la que aplicar la colisión
          /// @param block
          /// Bloque en el que está la partícula
          /// @param i
          /// Índice de la partícula en el bloque
          template <bool Side, Axis W>
          constexpr static void run(Block & block, std::size_t i) {
            double const d_w = (Side ? block.pos_dens(i).pos.get<W>() - Constants::b_min.get<W>()
                                     : Constants::b_max.get<W>() - block.pos_dens(i).pos.get<W>());
            if (d_w < 0) {
              block.pos_dens(i).pos.set<W>() =
                  (Side ? Constants::b_min.get<W>() - d_w : Constants::b_max.get<W>() + d_w);
              block.vel_acc(i).vel.set<W>() *= -1;
              block.hv(i).set<W>()          *= -1;
            }
          }
      };

      /// Itera por las partículas de un borde de la malla y les aplica una función
      /// @tparam Side
      /// Borde inferior (true) o superior (false)
      /// @tparam A
      /// Coordena externa de los bloques por la que iterar
      /// @tparam B
      /// Coordena interna de los bloques por la que iterar
      /// @tparam C
      /// Coordena fija de los bloques por los que iterar
      /// @tparam Proc
      /// Struct con la función a aplicar a cada partícula
      /// @param i
      /// Índice de la partícula en el bloque
      template <bool Side, Axis A, Axis B, Axis C, typename Proc>
      constexpr void wall_iterate() {
        Vector3<std::size_t> idx{0, 0, 0};
        // Calcula el valor de la coordenada fija y los límites de las otras
        idx.set<C>()              = (Side ? 0 : count_.get<C>() - 1);
        std::size_t const count_A = count_.get<A>();
        std::size_t const count_B = count_.get<B>();
        // Itera por las coordenadas A y B
        for (; idx.get<A>() < count_A; ++idx.set<A>()) {
          for (idx.set<B>() = 0; idx.get<B>() < count_B; ++idx.set<B>()) {
            // Itera por las partículas del bloque y les aplica la función
            Block & block = index(idx, front_);
            for (std::size_t i{0}; i < block.size(); ++i) { Proc::template run<Side, C>(block, i); }
          }
        }
      }

      /// Aplica una función a las partículas de cada borde
      /// @tparam Proc
      /// Struct con la función a aplicar a cada partícula
      template <typename Proc>
      constexpr void wall_action() {
        wall_iterate<true, Axis::y, Axis::x, Axis::z, Proc>();
        wall_iterate<false, Axis::y, Axis::x, Axis::z, Proc>();
        wall_iterate<true, Axis::z, Axis::x, Axis::y, Proc>();
        wall_iterate<false, Axis::z, Axis::x, Axis::y, Proc>();
        wall_iterate<true, Axis::z, Axis::y, Axis::x, Proc>();
        wall_iterate<false, Axis::z, Axis::y, Axis::x, Proc>();
      }

      /// Itera por los bloques adyacentes a uno dado, aplicando una función a cada uno
      /// @param f
      /// Función a aplicar a cada bloque
      template <typename F>
      void adjacent_blocks(Vector3<size_t> block, F f)
        requires std::invocable<F, Vector3<std::size_t> const &>
      // Para seleccionar cada par de bloques adyacentes exactamente 1 vez, se usa un conjunto
      // de bloques seleccionado cuidadosamente. Este conjunto debe cumplir 2 propiedades:
      // - Se deben seleccionar exactamente la mitad de los bloques adyacentes. En este caso:
      //   n = (3^3 - 1)/2 = 13
      // - Si se selecciona un bloque con desplazamiento (a, b, c) respecto del actual,
      //   el bloque con desplazamiento (-a, -b, -c) no se puede seleccionar
      //
      // El conjunto elegido que cumple estas propiedades es el siguiente:
      // (-1, -1, -1)
      // (-1, -1,  0)
      // (-1, -1,  1)
      // (-1,  0, -1)
      // (-1,  0,  0)
      // (-1,  0,  1)
      // (-1,  1, -1)
      // (-1,  1,  0)
      // (-1,  1,  1)
      // ( 0, -1, -1)
      // ( 0, -1,  0)
      // ( 0, -1,  1)
      // ( 0,  0, -1)
      {
        if (block.x > 0) {    // Offsets (-1, y, z)
          if (block.y > 0) {  // Offsets (-1, -1, z)
            if (block.z > 0) { f({block.x - 1, block.y - 1, block.z - 1}); }
            f({block.x - 1, block.y - 1, block.z});
            if (block.z < count_.z - 1) { f({block.x - 1, block.y - 1, block.z + 1}); }
          }
          // Offsets (-1, 0, z)
          if (block.z > 0) { f({block.x - 1, block.y, block.z - 1}); }
          f({block.x - 1, block.y, block.z});
          if (block.z < count_.z - 1) { f({block.x - 1, block.y, block.z + 1}); }
          if (block.y < count_.y - 1) {  // Offsets (-1, 1, z)
            if (block.z > 0) { f({block.x - 1, block.y + 1, block.z - 1}); }
            f({block.x - 1, block.y + 1, block.z});
            if (block.z < count_.z - 1) { f({block.x - 1, block.y + 1, block.z + 1}); }
          }
        }
        if (block.y > 0) {  // Offsets (0, -1, z)
          if (block.z > 0) { f({block.x, block.y - 1, block.z - 1}); }
          f({block.x, block.y - 1, block.z});
          if (block.z < count_.z - 1) { f({block.x, block.y - 1, block.z + 1}); }
        }
        // Offsets (0, 0, z)
        if (block.z > 0) { f({block.x, block.y, block.z - 1}); }
      }

      /// Itera por los bloques de la malla y aplica una función a cada uno
      /// @param f
      /// Función a aplicar a cada bloque
      template <typename F>
      constexpr void iter_blocks(F f)
        requires std::invocable<F, Vector3<std::size_t> const &>
      {
        Vector3<std::size_t> idx{0, 0, 0};
        for (; idx.z < count_.z; ++idx.z) {
          for (idx.y = 0; idx.y < count_.y; ++idx.y) {
            for (idx.x = 0; idx.x < count_.x; ++idx.x) { f(idx); }
          }
        }
      }

      /// Itera por pares de partículas en bloques adyacentes y les aplica una función
      template <typename F>
      constexpr void iter_pairs(F f)
        requires std::invocable<F, Block &, Block &, std::size_t, std::size_t>
      {
        // Itera por los bloques de la malla
        iter_blocks([&](Vector3<std::size_t> const & blockI_idx) {
          Block & blockI = index(blockI_idx, front_);
          // Si no tiene partículas, no hay que hacer nada y se pueden saltar el resto de
          // comprobaciones
          if (blockI.size() == 0) { return; }
          // Itera por los bloques adyacentes
          adjacent_blocks(blockI_idx, [&](Vector3<std::size_t> const & blockJ_idx) {
            Block & blockJ = index(blockJ_idx, front_);
            // Itera por todos los pares de partículas y les aplica la función
            for (std::size_t i{0}; i < blockI.size(); ++i) {
              for (std::size_t j{0}; j < blockJ.size(); ++j) { f(blockI, blockJ, i, j); }
            }
          });
          // Itera por todos los pares de partículas del bloque actual y les aplica la función
          for (std::size_t i{}; i < blockI.size(); ++i) {
            for (std::size_t j{i + 1}; j < blockI.size(); ++j) { f(blockI, blockI, i, j); }
          }
        });
      }

#ifdef FLUID_DEBUG

    public:
#else

    private:
#endif
      /// Calcula la densidad de las partículas
      constexpr void increment_densities() {
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
      /// Calcula la aceleración de las partículas
      constexpr void transfer_accelerations() {
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
      /// Mueve las partículas
      constexpr void particle_movement() {
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

      /// Calcula las interacciones de las partículas con las paredes
      constexpr void wall_interactions() { wall_action<interact_particle>(); }

      /// Calcula las colisiones de las partículas con las paredes
      constexpr void wall_collisions() { wall_action<collide_particle>(); }

      /// Reasigna las partículas a bloques
      constexpr void particle_repositioning() {
        // TODO: skip clear loop on the first iteration
        // Vacía todos los bloques en el búfer de la siguiente iteración
        for (auto & block : back_) { block.clear(); }
        // Itera por todas las partículas en el búfer de la iteración actual
        for (auto & block : front_) {
          for (std::size_t i{0}; i < block.size(); ++i) {
            // Añade la partícula a su bloque correspondiente en el búfer de la siguiente iteración
            Particle particle;
            particle.pos      = block.pos_dens(i).pos;
            particle.vel      = block.vel_acc(i).vel;
            particle.gradient = block.hv(i);
            particle.id       = block.id(i);
            particle.density  = reset_density_;
            find_block(particle.pos, back_).add(particle);
          }
        }
      }

      /// Intercambia los buffers de bloques
      constexpr void swap_buffers() { std::swap(front_, back_); }

      /// Devuelve los bloqus en la iteración actual
      [[nodiscard]] std::vector<Block> get_blocks() const { return front_; }

      /// Establece los bloques en la iteración actual a los dados (debug)
      void set_blocks(std::vector<Block> const & blocks) { front_ = blocks; }
  };

}  // namespace Fluid

#endif  // GRID_HPP
