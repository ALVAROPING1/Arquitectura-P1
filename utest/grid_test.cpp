// Pruebas unitarias para grid
#include "grid_test.hpp"

#include "sim/constants.hpp"
#include "sim/grid.hpp"
#include "sim/vector3.hpp"

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

namespace GridTest {

  // Test para asegurar que los calculos realizados son los correctos dentro de la clase
  // "Fluid::Grid" cuando se importan los datos de eun archivo.
  TEST_F(GridTestClass, InitOK) {
    Fluid::Grid grid{Const::num, Const::ppm};

    EXPECT_DOUBLE_EQ(grid.h(), Fluid::Constants::radius / Const::ppm);
    EXPECT_DOUBLE_EQ(grid.mass(),
                     Fluid::Constants::density / (Const::ppm * Const::ppm * Const::ppm));
    EXPECT_DOUBLE_EQ(grid.density_transform_factor_,
                     315 / (64 * std::numbers::pi * std::pow(grid.h(), 9)) * grid.mass());
    EXPECT_DOUBLE_EQ(grid.reset_density_, std::pow(grid.h(), 6) * 315 /
                                              (64 * std::numbers::pi * std::pow(grid.h(), 9)) *
                                              grid.mass());
    EXPECT_DOUBLE_EQ(grid.acceleration_transform_factor_, grid.mass() / std::pow(grid.h(), 6));
    EXPECT_EQ(grid.count(), static_cast<Fluid::Vector3<std::size_t>>(
                                (Fluid::Constants::b_max - Fluid::Constants::b_min) / grid.h()));
    EXPECT_EQ(grid.sizes(), (Fluid::Constants::b_max - Fluid::Constants::b_min) /
                                static_cast<Fluid::Vector3<double>>(grid.count()));
    ASSERT_EQ(grid.front_.size(), grid.count().x * grid.count().y * grid.count().z);
    ASSERT_EQ(grid.back_.size(), grid.count().x * grid.count().y * grid.count().z);
    ASSERT_EQ(grid.get_all().size(), 0);
  }

  // Test de la funcionalidad de añadir una partícula a la malla.
  // Se prueba que el número de particulas aumenta cuando se añaden particulas manualmente.
  TEST_F(GridTestClass, InsertParticle) {
    Fluid::Grid grid{Const::num, Const::ppm};

    grid.add(Const::pos, Const::hvel, Const::vel);

    EXPECT_EQ(grid.get_all().size(), 1);
    Fluid::Vector3<double> constexpr pos{static_cast<Fluid::Vector3<double>>(Const::pos)};

    Fluid::Vector3<double> const clamped_pos{
        pos.clamp(Fluid::Constants::b_min, Fluid::Constants::b_max - grid.sizes())};
    Fluid::Vector3<std::size_t> block_idx{static_cast<Fluid::Vector3<std::size_t>>(
        (clamped_pos - Fluid::Constants::b_min) / grid.sizes())};
    std::size_t idx{block_idx.z * grid.count().y * grid.count().x + block_idx.y * grid.count().x +
                    block_idx.x};
    EXPECT_EQ(grid.front_[idx].size(), 1);
    EXPECT_EQ(grid.back_[idx].size(), 0);
  }

};  // namespace GridTest
