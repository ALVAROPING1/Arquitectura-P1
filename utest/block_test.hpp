#ifndef BLOCK_TEST_HPP
#define BLOCK_TEST_HPP

#include "sim/block.hpp"
#include "sim/vector3.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

namespace BlockTest {
  namespace Const {
    // constante con el numero estimado de particulas en un bloque
    unsigned long const estimated_amount = 5;
    constexpr double density{2.0};
    constexpr Fluid::Vector3<double> pos{1.0, 1.5, 2.0};
    constexpr Fluid::Vector3<double> vel{0.5, -3.5, -3.0};
    constexpr Fluid::Vector3<double> acc{-2.0, 0, 1.0};
    constexpr Fluid::Vector3<double> gradient{-2.0, 0, 1.0};
  }  // namespace Const

  // clase de prueba para los bloques
  class BlockTestClass : public testing::Test {
    public:
      // funcion que devuelve el bloque de prueba
      [[nodiscard]] Fluid::Block & get_block() { return bloque_prueba; }

      // funcion que agrega una particula genérica al bloque prueba, devolviendola
      Fluid::Particle add_particle(int id) {
        Fluid::Particle particula{};
        particula.id       = id;
        particula.density  = Const::density;
        particula.pos      = Const::pos;
        particula.vel      = Const::vel;
        particula.acc      = Const::acc;
        particula.gradient = Const::gradient;
        bloque_prueba.add(particula);
        return particula;
      }

    private:
      // bloque de prueba
      Fluid::Block bloque_prueba{BlockTest::Const::estimated_amount};
  };

}  // namespace BlockTest
#endif  // BLOCK_TEST_HPP
