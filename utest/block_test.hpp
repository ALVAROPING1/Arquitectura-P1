#ifndef BLOCK_TEST_HPP
#define BLOCK_TEST_HPP

#include "../sim/block.hpp"

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
  }  // namespace Const

  // clase de prueba para los bloques
  class BlockTestClass : public testing::Test {
    public:
      // funcion que devuelve el bloque de prueba
      Fluid::Block get_block() { return bloque_prueba; }

      // funcion que agrega una particula generica al bloque prueba
      void add_particle() {
        Fluid::Particle particula{};
        particula.id = 1;
        bloque_prueba.add(particula);
      }

    private:
      // bloque de prueba
      Fluid::Block bloque_prueba{BlockTest::Const::estimated_amount};
  };

}  // namespace BlockTest
#endif  // BLOCK_TEST_HPP