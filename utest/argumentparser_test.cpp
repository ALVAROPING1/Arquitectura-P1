// Pruebas unitarias para progargs
//
#include "sim/argumentparser.hpp"
#include "sim/io.hpp"

#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>

namespace ArgumentParserTest {
  int constexpr items_args = 4;

  // Resultado correcto
  TEST(ArgumentParserDeathTest, OK) {
    // Lista de argumentos para el test de ok.
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld")};

    ASSERT_EXIT(
        {
          Fluid::ArgumentParser const parser{args};
          std::exit(0);
        },
        testing::ExitedWithCode(0), "");
  }

  // Menos argumentos
  TEST(ArgumentParserDeathTest, LessArgs) {
    // Lista de argumentos para el test con menos argumentos.
    std::array<gsl::czstring, items_args - 1> args = {static_cast<gsl::czstring>("fluid"),
                                                      static_cast<gsl::czstring>("5"),
                                                      static_cast<gsl::czstring>("in.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1),
                "Error: Invalid number of arguments: 2");
  }

  // Más argumentos
  TEST(ArgumentParserDeathTest, MoreArgs) {
    // Lista de argumentos para el test con mas argumentos.
    std::array<gsl::czstring, items_args + 1> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld"),
        static_cast<gsl::czstring>("60")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1),
                "Error: Invalid number of arguments: 4");
  }

  // Parámetro de iteraciones no digitos
  TEST(ArgumentParserDeathTest, InvIterations) {
    // Lista de argumentos para el test con numero de iteraciones no numericas.
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("byebye"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -1), "Error: time steps must be numeric.");
  }

  // Iteraciones negativas. Comparar error con -2.
  TEST(ArgumentParserDeathTest, NegIterations) {
    // Lista de argumentos para el test con numero de iterationes negativo
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("-1"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld")};

    ASSERT_EXIT(Fluid::ArgumentParser const parser{args},
                testing::ExitedWithCode((unsigned char) -2),
                "Error: Invalid number of time steps.");
  }

  // Iteraciones 0
  TEST(ArgumentParserDeathTest, ZeroIterations) {
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("0"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld")};

    ASSERT_EXIT(
        {
          Fluid::ArgumentParser const parser{args};
          std::exit(0);
        },
        testing::ExitedWithCode(0), "");
  }

  // Getters
  TEST(ArgumentParserTest, Getters) {
    // Lista de argumentos para el test de ok.
    std::array<gsl::czstring, items_args> args = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("3"),
        static_cast<gsl::czstring>("in.fld"), static_cast<gsl::czstring>("out.fld")};
    Fluid::ArgumentParser const parser{args};
    EXPECT_EQ(parser.step_count(), 3);
    EXPECT_EQ(parser.in(), "in.fld");
    EXPECT_EQ(parser.out(), "out.fld");
  }
};  // namespace ArgumentParserTest
