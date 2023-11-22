// Pruebas unitarias para grid
#include "sim/block.hpp"
#include "sim/constants.hpp"
#include "sim/grid.hpp"
#include "sim/particle.hpp"
#include "sim/vector3.hpp"

#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

namespace GridTest {
  // Espacio virtual de constantes para la creación de archivos custom.
  // Incluye constantes para el número de partículas por metro y para los
  // diferentes números de particulas a inyectar en los archivos custom. También,
  // contiene unos vectores de la clase de "Fluid::Vector3<float>" que representan
  // la posición, el gradiente y la velocidad de cada partícula.
  namespace Const {
    // constantes de pruebas.
    float constexpr ppm = 75;
    int constexpr num   = 5;

    // vectores de ejemplo para las partículas de prueba.
    Fluid::Vector3<float> constexpr vel{1, 1, 1};
    Fluid::Vector3<float> constexpr pos{0, 0, 0};
    Fluid::Vector3<float> constexpr hvel{1, 1, 1};

    // vectores de posición fuera del recinto
    Fluid::Vector3<float> constexpr pos_inf_x{-1, 0, 0};
    Fluid::Vector3<float> constexpr pos_inf_y{0, -1, 0};
    Fluid::Vector3<float> constexpr pos_inf_z{0, 0, -1};
    Fluid::Vector3<float> constexpr pos_sup_x{1, 0, 0};
    Fluid::Vector3<float> constexpr pos_sup_y{0, 1, 0};
    Fluid::Vector3<float> constexpr pos_sup_z{0, 0, 1};

  };  // namespace Const

  // Test para la inicialización/getters de constantes
  TEST(GridTest, InitOK) {
    Fluid::Grid const grid{Const::num, Const::ppm};

    ASSERT_DOUBLE_EQ(grid.h(), Fluid::Constants::radius / Const::ppm);
    ASSERT_DOUBLE_EQ(grid.mass(),
                     Fluid::Constants::density / (Const::ppm * Const::ppm * Const::ppm));
    ASSERT_EQ(grid.count(), static_cast<Fluid::Vector3<std::size_t>>(
                                (Fluid::Constants::b_max - Fluid::Constants::b_min) / grid.h()));
    EXPECT_EQ(grid.sizes(), (Fluid::Constants::b_max - Fluid::Constants::b_min) /
                                static_cast<Fluid::Vector3<double>>(grid.count()));
    EXPECT_EQ(grid.get_all().size(), 0);
    EXPECT_EQ(grid.get_blocks().size(), grid.count().x * grid.count().y * grid.count().z);
  }

  // Test para get_all
  TEST(GridTest, get_all) {
    Fluid::Grid grid{Const::num, Const::ppm};
    std::vector<Fluid::Block> blocks;
    blocks.resize(3, Fluid::Block{1});
    blocks[1].add(Fluid::Particle{});
    grid.set_blocks(blocks);
    std::vector<Fluid::Particle> expected;
    expected.emplace_back();
    auto result = grid.get_all();
    EXPECT_EQ(result, expected);
  }

  // Test para get_blocks/set_blocks
  TEST(GridTest, get_set_blocks) {
    Fluid::Grid grid{Const::num, Const::ppm};
    std::vector<Fluid::Block> expected;
    expected.resize(3, Fluid::Block{1});
    expected[1].add(Fluid::Particle{});
    grid.set_blocks(expected);
    ASSERT_EQ(grid.get_blocks().size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(grid.get_blocks()[i].get_particles(), expected[i].get_particles());
    }
  }

  // Test de la funcionalidad de añadir una partícula a la malla.
  // Se prueba que el número de particulas aumenta cuando se añaden
  // particulas manualmente, y que se añaden al bloque correcto
  void test_add_particle(Fluid::Vector3<float> pos) {
    Fluid::Grid grid{Const::num, Const::ppm};
    grid.add(pos, Const::hvel, Const::vel);
    ASSERT_EQ(grid.get_all().size(), 1);
    std::size_t const idx = grid.index(static_cast<Fluid::Vector3<double>>(pos));
    EXPECT_EQ(grid.get_blocks()[idx].size(), 1);
  }

  // Test de añadir partículas a la malla con cada uno de los posibles casos
  // de la posición de la partícula en relación a los bordes de la malla
  TEST(GridTest, InsertParticle) {
    test_add_particle(Const::pos);
    test_add_particle(Const::pos_inf_x);
    test_add_particle(Const::pos_inf_y);
    test_add_particle(Const::pos_inf_z);
    test_add_particle(Const::pos_sup_x);
    test_add_particle(Const::pos_sup_y);
    test_add_particle(Const::pos_sup_z);
  }

  // Test de indexar la malla con las coordenadas de un bloque
  TEST(GridTest, index_block_coordinate) {
    Fluid::Grid const grid{Const::num, Const::ppm};
    constexpr Fluid::Vector3<std::size_t> idx{1, 2, 3};
    ASSERT_EQ(grid.index(idx),
              idx.z * grid.count().y * grid.count().x + idx.y * grid.count().x + idx.x);
  }

  // Test de indexar la malla con la posición de una partícula
  void test_index_pos(Fluid::Vector3<double> pos) {
    Fluid::Grid const grid{Const::num, Const::ppm};
    Fluid::Vector3<double> const clamped_pos{
      pos.clamp(Fluid::Constants::b_min, Fluid::Constants::b_max - grid.sizes())};
    EXPECT_EQ(grid.index(pos), grid.index(static_cast<Fluid::Vector3<std::size_t>>(
                                   (clamped_pos - Fluid::Constants::b_min) / grid.sizes())));
  }

  // Test de indexar la malla con posiciones de partículas en cada
  // uno de los posibles casos en relación a los bordes de la malla
  TEST(GridTest, index_particle_position) {
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_inf_x));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_inf_y));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_inf_z));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_sup_x));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_sup_y));
    test_index_pos(static_cast<Fluid::Vector3<double>>(Const::pos_sup_z));
  }
};  // namespace GridTest
