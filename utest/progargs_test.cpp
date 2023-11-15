// Pruebas unitarias para progargs
//
#include "../sim/argumentparser.hpp"
#include "../sim/io.hpp"
#include "FileIO_test.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>
#include <span>

namespace ArgumentParserTest {  // No se incluye las pruebas de testeo de los archivos
                                // ya que se indicaron en las pruebas unitarias de
                                // FileIO.
  using ArgumentParserTestClass          = FileIOTest::FileIOTestClass;
  using ArgumentParserTestClassDeathTest = ArgumentParserTestClass;
  int const items_args                   = 4;

  // Resultado correcto
  TEST_F(ArgumentParserTestClassDeathTest, OK) {
    // Lista de argumentos para el test de ok.
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("NpTrue.fld"), static_cast<gsl::czstring>("out.fld")};

    ASSERT_EXIT(
        {
          Fluid::ArgumentParser const parser{args};
          std::exit(0);
        },
        testing::ExitedWithCode(0), "");
  }

  // Menos argumentos
  TEST_F(ArgumentParserTestClassDeathTest, LessArgs) {
    // Lista de argumentos para el test con menos argumentos.
    std::array<gsl::czstring, items_args - 1> args = {static_cast<gsl::czstring>("fluid"),
                                                      static_cast<gsl::czstring>("5"),
                                                      static_cast<gsl::czstring>("NpTrue.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1),
                "Error: Invalid number of arguments: 2");
  }

  // Más argumentos
  TEST_F(ArgumentParserTestClassDeathTest, MoreArgs) {
    // Lista de argumentos para el test con mas argumentos.
    std::array<gsl::czstring, items_args + 1> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("NpTrue.fld"), static_cast<gsl::czstring>("final.fld"),
        static_cast<gsl::czstring>("60")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1),
                "Error: Invalid number of arguments: 4");
  }

  // Parámetro de iteraciones no digitos
  TEST_F(ArgumentParserTestClassDeathTest, InvIterations) {
    // Lista de argumentos para el test con numero de iteraciones no numericas.
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("byebye"),
        static_cast<gsl::czstring>("NpTrue.fld"), static_cast<gsl::czstring>("final.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1), "Error: time steps must be numeric.");
  }

  // Iteraciones negativas. Comparar error con -2.
  TEST_F(ArgumentParserTestClassDeathTest, NegIterations) {
    // Lista de argumentos para el test con numero de iterationes negativo
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("-1"),
        static_cast<gsl::czstring>("NpTrue.fld"), static_cast<gsl::czstring>("final.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -2),
                "Error: Invalid number of time steps.");
  }

};  // namespace ArgumentParserTest

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
