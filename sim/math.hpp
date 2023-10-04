#ifndef MATH_HPP
#define MATH_HPP

#include "constants.hpp"
#include "vector3.hpp"

#include <array>
#include <cmath>
#include <iostream>

namespace Fluid::Math {

  constexpr double calc_particle_mass(double ppm) {
    return Fluid::Constants::rho * pow(ppm, 3);
  }

  // XXX: Adri, todavía no nos hemos repartido esta parte. Termina primero tu
  //      clase vector y Partícula primero.
  //
  //      Además, calc_particle_block_index, es cosa de Grid, no de Math.
  //      Porque Grid guarda un estado que evita tener que recalcular muchas
  //      cosas.
  //
  // XXX: En este caso no es necesario utilizar templetización porque se nos ha
  //      dicho que trabajamos solo con doubles. En este caso no es buena idea
  //      utilizar std::array, porque es necesario llamar al constructor y la
  //      posición de particle_pos la tenemos ya en vector3
  //
  // XXX: La implementación se moverá a Grid o a la clase pertinente cuando sea
  //      necesario.
  //
  // XXX: Porfi, formatea el archivo con .clang-format antes de subir nada.
  template <class T>
  std::array<int, 3> calc_particle_block_index(std::array<T, 3> const & particle_pos,
                                               std::array<T, 3> const & block_min_position,
                                               std::array<T, 3> const & size_block_grid) {
    std::array<int, 3> indexes{
      static_cast<int>((particle_pos[0] - block_min_position[0]) / size_block_grid[0]),
      static_cast<int>((particle_pos[1] - block_min_position[1]) / size_block_grid[1]),
      static_cast<int>((particle_pos[2] - block_min_position[2]) / size_block_grid[2]),
    };

    return indexes;
  }

  constexpr double calc_h(double ppm) {
    return Fluid::Constants::r / ppm;
  }

  Fluid::Vector3<double> calc_n_blocks(Fluid::Vector3<double> vmax, Fluid::Vector3<double> vmin,
                                       double h) {
    return Vector3<double>{(vmax.x - vmin.x) / h, (vmax.y - vmin.y) / h, (vmax.z - vmin.z) / h};
  }

  Fluid::Vector3<double> calc_size_block_grid(Fluid::Vector3<double> vmax,
                                              Fluid::Vector3<double> vmin,
                                              Fluid::Vector3<double> n_bloques) {
    return Vector3<double>{(vmax.x - vmin.x) / n_bloques.x, (vmax.y - vmin.y) / n_bloques.y,
                           (vmax.z - vmin.z) / n_bloques.z};
  }

}  // namespace Fluid::Math

#endif
