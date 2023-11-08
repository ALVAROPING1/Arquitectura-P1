// Pruebas unitarias para block
#include "block_test.hpp"

#include "../sim/block.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace BlockTest {
  // prueba add
  // clases de equivalencia
  TEST_F(BlockTestClass, agregar_particula) {
    Fluid::Particle expected_answer = {};
    expected_answer.id              = 4;
    Fluid::Block bloque             = get_block();
    bloque.add(expected_answer);
    Fluid::Particle const actual_answer = bloque[0];
    ASSERT_EQ(actual_answer.id, expected_answer.id);
  }

  // prueba clear
  // clases de equivalencia
  TEST_F(BlockTestClass, quitar_particulas) {
    int const expected_answer = 0;
    Fluid::Block bloque       = get_block();
    add_particle();
    bloque.clear();
    ASSERT_EQ(bloque.size(), expected_answer);
  }

  // prueba indexar
  // clases de equivalencia
  TEST_F(BlockTestClass, operador_indexar) {
    Fluid::Particle expected_answer = {};
    expected_answer.id              = 4;
    add_particle();
    Fluid::Block bloque = get_block();
    bloque.add(expected_answer);

    Fluid::Particle const actual_answer = bloque[1];
    ASSERT_EQ(actual_answer.id, expected_answer.id);
  }

  // prueba size
  // clases de equivalencia
  TEST_F(BlockTestClass, numero_elementos) {
    int const expected_answer = 10;
    for (int i = 0; i < expected_answer; i++) { add_particle(); }
    Fluid::Block const bloque = get_block();
    ASSERT_EQ(bloque.size(), expected_answer);
  }

  // get particles
  // clases de equivalencia
  TEST_F(BlockTestClass, obtener_particulas) {
    std::vector<Fluid::Particle> expected_answer;
    Fluid::Block bloque = get_block();
    int const count     = 5;
    for (int i = 0; i < count; i++) {
      Fluid::Particle particula{};
      particula.id = i;
      expected_answer.push_back(particula);
      bloque.add(particula);
    }
    // comparar vectores
    std::vector<Fluid::Particle> const actual_answer = bloque.get_particles();
    bool equal                                       = true;
    for (int i = 0; i < count; i++) {
      if (expected_answer[i].id != actual_answer[i].id) { equal = false; }
    }
    ASSERT_EQ(equal, true);
  }

  // funcion id
  // clases de equivalencia
  TEST_F(BlockTestClass, id_particula) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.id(0), particula.id);
  }

  // funcion hv
  // clases de equivalencia
  TEST_F(BlockTestClass, funcion_hv) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.hv(0), particula.gradient);
  }

  // funcion vel_acc
  // clases de equivalencia
  // dividido en dos
  // comparar la aceleracion
  TEST_F(BlockTestClass, funcion_vel_acc_1) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.vel_acc(0).acc, particula.acc);
  }

  // comparar la velocidad
  TEST_F(BlockTestClass, funcion_vel_acc_2) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.vel_acc(0).vel, particula.vel);
  }

  // funcion pos_dens
  // clases de equivalencia
  // dividido en dos
  // comparar la posicion
  TEST_F(BlockTestClass, funcion_pos_dens_1) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.pos_dens(0).pos, particula.pos);
  }

  // comparar la densidad
  TEST_F(BlockTestClass, funcion_pos_dens_2) {
    Fluid::Particle particula{};
    int const id_particula = 5;
    particula.id           = id_particula;
    Fluid::Block bloque    = get_block();
    bloque.add(particula);
    ASSERT_EQ(bloque.pos_dens(0).dens, particula.density);
  }

}  // namespace BlockTest

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
