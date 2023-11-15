// Pruebas unitarias para grid
#include "grid_test.hpp"

#include "sim/constants.hpp"
#include "sim/fileio.hpp"
#include "sim/grid.hpp"
#include "sim/vector3.hpp"

#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

namespace GridTest {

  // Test para asegurar que los calculos realizados son los correctos dentro de la clase
  // "Fluid::Grid" cuando se importan los datos de eun archivo.
  TEST_F(GridTestClass, ImportOK) {
    Fluid::FileIO iotest{};
    iotest.open("NpTrue.fld", "out.fld");

    Fluid::Grid test_grid{iotest.np(), iotest.ppm()};
    iotest.load(test_grid);

    EXPECT_EQ(test_grid.h(), Fluid::Constants::radius / iotest.ppm());
    EXPECT_EQ(test_grid.mass(),
              Fluid::Constants::density / (iotest.ppm() * iotest.ppm() * iotest.ppm()));
    EXPECT_EQ(test_grid.acceleration_transform_factor_,
              test_grid.mass() / std::pow(test_grid.h(), 2 * 3));
    EXPECT_EQ(test_grid.count(),
              static_cast<Fluid::Vector3<std::size_t>>(
                  (Fluid::Constants::b_max - Fluid::Constants::b_min) / test_grid.h()));
    EXPECT_EQ(test_grid.sizes(), (Fluid::Constants::b_max - Fluid::Constants::b_min) /
                                     static_cast<Fluid::Vector3<double>>(test_grid.count()));
    ASSERT_EQ(test_grid.get_all().size(), Const::it_np_true);
  }

  // Test de la funcionalidad de añadir una partícula a la malla.
  // Se prueba que el número de particulas aumenta cuando se añaden particulas manualmente.
  TEST_F(GridTestClass, InsertParticle) {
    Fluid::FileIO iotest{};
    iotest.open("NpTrue.fld", "out.fld");

    Fluid::Grid test_grid{iotest.np(), iotest.ppm()};
    iotest.load(test_grid);

    EXPECT_EQ(test_grid.get_all().size(), Const::it_np_true);

    for (int i = 0; i < 4; i++) { test_grid.add(Const::pos, Const::hvel, Const::vel); }

    EXPECT_EQ(test_grid.get_all().size(), Const::it_np_true + 4);
  }

};  // namespace GridTest

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
