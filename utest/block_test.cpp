// Pruebas unitarias para block
#include "block_test.hpp"

#include "sim/block.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace BlockTest {
  // prueba add
  // clases de equivalencia
  TEST_F(BlockTestClass, agregar_particula) {
    Fluid::Particle const expected = add_particle(1);
    ASSERT_EQ(get_block()[0], expected);
  }

  // prueba clear
  // clases de equivalencia
  TEST_F(BlockTestClass, quitar_particulas) {
    Fluid::Block & bloque = get_block();
    ASSERT_EQ(bloque.size(), 0);
    add_particle(1);
    ASSERT_EQ(bloque.size(), 1);
    bloque.clear();
    ASSERT_EQ(bloque.size(), 0);
  }

  // prueba indexar
  // clases de equivalencia
  TEST_F(BlockTestClass, operador_indexar) {
    add_particle(2);
    Fluid::Particle const expected = add_particle(2);
    ASSERT_EQ(get_block()[1], expected);
  }

  // prueba size
  // clases de equivalencia
  TEST_F(BlockTestClass, numero_elementos) {
    int const expected_answer = 10;
    for (int i = 0; i < expected_answer; i++) { add_particle(i); }
    ASSERT_EQ(get_block().size(), expected_answer);
  }

  // get particles
  // clases de equivalencia
  TEST_F(BlockTestClass, obtener_particulas) {
    std::vector<Fluid::Particle> expected_answer;
    int const count = 5;
    for (int i = 0; i < count; i++) {
      Fluid::Particle const particula = add_particle(i);
      expected_answer.push_back(particula);
    }
    // comparar vectores
    std::vector<Fluid::Particle> const actual_answer = get_block().get_particles();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  // funcion id
  // clases de equivalencia
  TEST_F(BlockTestClass, id_particula) {
    Fluid::Particle const particula = add_particle(4);
    ASSERT_EQ(get_block().id(0), particula.id);
  }

  // funcion gradient
  // clases de equivalencia
  TEST_F(BlockTestClass, funcion_gradient) {
    Fluid::Particle const particula = add_particle(4);
    ASSERT_EQ(get_block().gradient(0), particula.gradient);
  }

  // funcion vel_acc
  // clases de equivalencia
  TEST_F(BlockTestClass, funcion_vel_acc_1) {
    Fluid::Particle const particula = add_particle(4);
    ASSERT_EQ(get_block().vel_acc(0).vel, particula.vel);
    ASSERT_EQ(get_block().vel_acc(0).acc, particula.acc);
  }

  // funcion pos_dens
  // clases de equivalencia
  TEST_F(BlockTestClass, funcion_pos_dens_1) {
    Fluid::Particle const particula = add_particle(4);
    ASSERT_EQ(get_block().pos_dens(0).pos, particula.pos);
    ASSERT_EQ(get_block().pos_dens(0).dens, particula.density);
  }

}  // namespace BlockTest

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
