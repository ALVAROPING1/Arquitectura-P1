// Representación de la malla
#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "constants.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <cmath>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

namespace Fluid {

  // Objeto representando la malla de la simulación
  class Grid {
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

    public:
      /// Construye una nueva malla vacía.
      /// @param pc
      /// Número de partículas
      /// @param ppm
      /// Partículas por metro
      constexpr Grid(std::size_t pc, double ppm)
        : h_{Constants::r / ppm}, m_{Constants::rho / (ppm * ppm * ppm)},
          density_transform_factor_{density_factor / std::pow(h_, 3 * 3) * m_},
          acceleration_transform_factor_{m_ / std::pow(h_, 2 * 3)},
          reset_density_{density_factor / std::pow(h_, 3) * m_},
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
      /// El gradiente de la velocidad de la partícula
      /// @param v
      /// La velocidad de la partícula
      void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v) {
        Particle const particle{
            {p, hv, v},
            amount_++, reset_density_
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

      /// Devuelve todas las partículas de la malla
      [[nodiscard]] std::vector<Particle> get_all() const;

#ifdef FLUID_DEBUG

    public:
#else

    private:
#endif
      /// Reasigna las partículas a bloques
      void particle_repositioning();
      /// Calcula la densidad de las partículas
      void increment_densities();
      /// Calcula la aceleración de las partículas
      void transfer_accelerations();
      /// Mueve las partículas
      void particle_movement();

      /// Calcula las interacciones de las partículas con las paredes
      constexpr void wall_interactions() { wall_action<interact_particle>(); }

      /// Calcula las colisiones de las partículas con las paredes
      constexpr void wall_collisions() { wall_action<collide_particle>(); }

      /// Intercambia los buffers de bloques
      constexpr void swap_buffers() { std::swap(front_, back_); }

      /// Devuelve los bloqus en la iteración actual
      [[nodiscard]] std::vector<Block> get_blocks() const { return front_; }

      /// Establece los bloques en la iteración actual a los dados (debug)
      void set_blocks(std::vector<Block> const & blocks) { front_ = blocks; }

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
          /// Borde inferior (0) o superior (1)
          /// @tparam Axis
          /// Struct de funciones getter/setter de una coordenada de un vector
          /// (`Axis_X`/`Axis_Y`/`Axis_Z`)
          /// @param block
          /// Bloque en el que está la partícula
          /// @param i
          /// Índice de la partícula en el bloque
          template <int Side, typename Axis>
          constexpr static void run(Block & block, std::size_t i) {
            constexpr double epsilon = 1.0e-10;
            // NOLINTBEGIN (readability-identifier-length) Mathematical equation
            double const w =
                Axis::get(block.pos_dens(i).pos) + Axis::get(block.hv(i)) * Constants::dt;
            double const dw = Constants::d_p - (Side == 0 ? w - Axis::get(Constants::b_min)
                                                          : Axis::get(Constants::b_max) - w);
            // NOLINTEND (readability-identifier-length)
            if (dw > epsilon) {
              Axis::set(block.vel_acc(i).acc) += (Side == 0 ? +1.0 : -1.0) * Constants::c_s * dw -
                                                 Constants::d_v * Axis::get(block.vel_acc(i).vel);
            }
          }
      };

      /// Calcula la colisión de una partícula con el borde dado
      struct collide_particle {
          /// Ejecuta el cálculo
          /// @tparam Side
          /// Borde inferior (0) o superior (1)
          /// @tparam Axis
          /// Struct de funciones getter/setter de una coordenada de un vector
          /// (`Axis_X`/`Axis_Y`/`Axis_Z`)
          /// @param block
          /// Bloque en el que está la partícula
          /// @param i
          /// Índice de la partícula en el bloque
          template <int Side, typename Axis>
          constexpr static void run(Block & block, std::size_t i) {
            double const d_w =
                (Side == 0 ? Axis::get(block.pos_dens(i).pos) - Axis::get(Constants::b_min)
                           : Axis::get(Constants::b_max) - Axis::get(block.pos_dens(i).pos));
            if (d_w < 0) {
              Axis::set(block.pos_dens(i).pos)  = (Side == 0 ? Axis::get(Constants::b_min) - d_w
                                                             : Axis::get(Constants::b_max) + d_w);
              Axis::set(block.vel_acc(i).vel)  *= -1;
              Axis::set(block.hv(i))           *= -1;
            }
          }
      };

      /// Itera por las partículas de un borde de la malla y les aplica una función
      /// @tparam Side
      /// Borde inferior (0) o superior (1)
      /// @tparam A
      /// Coordena externa de los bloques por la que iterar.
      /// Struct de funciones getter/setter de una coordenada de un vector
      /// (`Axis_X`/`Axis_Y`/`Axis_Z`)
      /// @tparam B
      /// Coordena interna de los bloques por la que iterar.
      /// Struct de funciones getter/setter de una coordenada de un vector
      /// (`Axis_X`/`Axis_Y`/`Axis_Z`)
      /// @tparam C
      /// Coordena fija de los bloques por los que iterar.
      /// Struct de funciones getter/setter de una coordenada de un vector
      /// (`Axis_X`/`Axis_Y`/`Axis_Z`)
      /// @tparam Proc
      /// Struct con la función a aplicar a cada partícula
      /// @param i
      /// Índice de la partícula en el bloque
      template <int Side, typename A, typename B, typename C, typename Proc>
      constexpr void wall_iterate() {
        Vector3<std::size_t> idx{0, 0, 0};
        // Calcula el valor de la coordenada fija y los límites de las otras
        C::set(idx)               = (Side == 0 ? 0 : C::get(count_) - 1);
        std::size_t const count_A = A::get(count_);
        std::size_t const count_B = B::get(count_);
        // Itera por las coordenadas A y B
        for (; A::get(idx) < count_A; ++A::set(idx)) {
          for (B::set(idx) = 0; B::get(idx) < count_B; ++B::set(idx)) {
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
        wall_iterate<0, Axis_Y, Axis_X, Axis_Z, Proc>();
        wall_iterate<1, Axis_Y, Axis_X, Axis_Z, Proc>();
        wall_iterate<0, Axis_Z, Axis_X, Axis_Y, Proc>();
        wall_iterate<1, Axis_Z, Axis_X, Axis_Y, Proc>();
        wall_iterate<0, Axis_Z, Axis_Y, Axis_X, Proc>();
        wall_iterate<1, Axis_Z, Axis_Y, Axis_X, Proc>();
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
  };

}  // namespace Fluid

#endif  // GRID_HPP
