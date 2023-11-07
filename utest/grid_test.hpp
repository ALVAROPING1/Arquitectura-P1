#pragma once

#include "../sim/io.hpp"

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sys/stat.h>

namespace GridTest {

  namespace Const {

    float const ppm = 75;
    int const f_np  = 5;
    Fluid::Vector3<float> const vel{1, 1, 1};
    Fluid::Vector3<float> const pos{1, 1, 1};
    Fluid::Vector3<float> const hvel{1, 1, 1};

    int const it_np_true = 5;
  };  // namespace Const

  class GridTestClass : public testing::Test {
      static bool prepare_np_true(std::string const & name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
          std::cout << "El archivo" << name << " no se ha podido crear\n";
          return false;
        }

        Fluid::IO::write(file, Const::ppm);
        Fluid::IO::write(file, Const::f_np);

        for (int i = 0; i < Const::it_np_true; i++) {
          Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        file.close();
        return true;
      };

      void SetUp() override {
        bool const result_3 = prepare_np_true("NpTrue.fld");

        if (!result_3) {
          std::cout << "Las pruebas no se prepararon correctamente: \n";
        } else {
          std::cout << "Los archivos se crearon correctamente!!\n";
        };
      };

      void TearDown() override {
        int const rem3 = std::remove("NpTrue.fld");

        if (rem3 != 0) {
          std::cout << "Los archivos de tests FileIO no se han podido borrar!!\n";
        } else {
          std::cout << "Los archivos se borraron correctamente!!\n";
        };
      };
  };

}  // namespace GridTest
