// Pruebas unitarias para grid
#include "grid_test.hpp"

#include "sim/constants.hpp"
#include "sim/grid.hpp"
#include "sim/vector3.hpp"

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

namespace GridTest {

  [[nodiscard]] std::size_t get_index(Fluid::Vector3<double> const & pos,
                                      Fluid::Vector3<double> const & grid_size,
                                      Fluid::Vector3<std::size_t> const & grid_count) {
    Fluid::Vector3<double> const clamped_pos{
        pos.clamp(Fluid::Constants::b_min, Fluid::Constants::b_max - grid_size)};
    Fluid::Vector3<std::size_t> const block_idx{static_cast<Fluid::Vector3<std::size_t>>(
        (clamped_pos - Fluid::Constants::b_min) / grid_size)};
    return block_idx.z * grid_count.y * grid_count.x + block_idx.y * grid_count.x + block_idx.x;
  }

  // Test para asegurar que los calculos realizados son los correctos dentro de la clase
  // "Fluid::Grid" cuando se importan los datos de eun archivo.
  TEST_F(GridTestClass, InitOK) {
    Fluid::Grid const grid{Const::num, Const::ppm};

    ASSERT_DOUBLE_EQ(grid.h(), Fluid::Constants::radius / Const::ppm);
    ASSERT_DOUBLE_EQ(grid.mass(),
                     Fluid::Constants::density / (Const::ppm * Const::ppm * Const::ppm));
    EXPECT_DOUBLE_EQ(grid.density_transform_factor_,
                     315 / (64 * std::numbers::pi * std::pow(grid.h(), 9)) * grid.mass());
    EXPECT_DOUBLE_EQ(grid.reset_density_, std::pow(grid.h(), 6) * 315 /
                                              (64 * std::numbers::pi * std::pow(grid.h(), 9)) *
                                              grid.mass());
    EXPECT_DOUBLE_EQ(grid.acceleration_transform_factor_, grid.mass() / std::pow(grid.h(), 6));
    ASSERT_EQ(grid.count(), static_cast<Fluid::Vector3<std::size_t>>(
                                (Fluid::Constants::b_max - Fluid::Constants::b_min) / grid.h()));
    EXPECT_EQ(grid.sizes(), (Fluid::Constants::b_max - Fluid::Constants::b_min) /
                                static_cast<Fluid::Vector3<double>>(grid.count()));
    EXPECT_EQ(grid.front_.size(), grid.count().x * grid.count().y * grid.count().z);
    EXPECT_EQ(grid.back_.size(), grid.count().x * grid.count().y * grid.count().z);
    EXPECT_EQ(grid.get_all().size(), 0);
  }

  // Test de la funcionalidad de añadir una partícula a la malla.
  // Se prueba que el número de particulas aumenta cuando se añaden
  // particulas manualmente, y que se añaden al bloque correcto
  void test_add_particle(Fluid::Vector3<float> pos) {
    Fluid::Grid grid{Const::num, Const::ppm};
    grid.add(pos, Const::hvel, Const::vel);
    ASSERT_EQ(grid.get_all().size(), 1);
    std::size_t const idx =
        get_index(static_cast<Fluid::Vector3<double>>(pos), grid.sizes(), grid.count());
    EXPECT_EQ(grid.front_[idx].size(), 1);
    EXPECT_EQ(grid.back_[idx].size(), 0);
  }

  TEST_F(GridTestClass, InsertParticle) {
    test_add_particle(Const::pos);
    test_add_particle(Const::pos_inf_x);
    test_add_particle(Const::pos_inf_y);
    test_add_particle(Const::pos_inf_z);
    test_add_particle(Const::pos_sup_x);
    test_add_particle(Const::pos_sup_y);
    test_add_particle(Const::pos_sup_z);
  }

};  // namespace GridTest
