// Representación de la malla
#ifndef __GRID_HPP
#define __GRID_HPP

#include "block.hpp"
#include "particle.hpp"
#include "vector3.hpp"

#include <array>
#include <vector>

namespace Fluid {

  class Grid {
      using block_vector = std::vector<Block>;
      Vector3<std::size_t> const count_;
      Vector3<std::size_t> const sizes_;
      std::array<block_vector> blocks_;
      block_vector const & front;
      block_vector & back;

    public:
      // Construye una nueva malla vacía.
      // @param pc
      // Número de partículas
      // @param ppm
      // Partículas por metro
      constexpr Grid(std::size_t pc, double ppm);

      // Añade una partícula a la malla.
      // @param p
      // La posición de la partícula
      // @param hv
      // El parámetro hv de la partícula
      // @param v
      // La velocidad de la partícula
      constexpr void add(Vector3<float> p, Vector3<float> hv, Vector3<float> v);

      // Ejecuta una iteración (paso) de la simulación.
      void step();

    private:
      void swap_buffers();

    protected:
      // Dada una posición devuelve el bloque donde se encuentra la partícula.
      // @param pos
      // La posición de la partícula
      constexpr Block & index(Vector3<double> pos);
  };

}  // namespace Fluid

#endif  //__GRID_HPP
