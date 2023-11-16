// funcion main customizada para tests

#include "fileio_test.hpp"

#include "sim/fileio.hpp"
#include "sim/grid.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace FileIOTest {
  using FileIOTestClassDeathTest = FileIOTestClass;
  constexpr int DoubleFormatPrecision{8};

  // Comprueba si 2 archivos son idénticos byte a byte
  [[nodiscard]] testing::AssertionResult compare_files(std::string const & left,
                                                       std::string const & right) {
    std::ifstream left_file{left};
    std::ifstream right_file{right};
    if (!left_file) {
      return testing::AssertionFailure() << "Couldn't open file " << left << "(correct)";
    }
    if (!right_file) {
      return testing::AssertionFailure() << "Couldn't open file " << right << "(calculated)";
    }
    // Compara los archivos byte a byte
    char correct_byte{};
    char result_byte{};
    while (left_file.read(&result_byte, sizeof(char)) and
           right_file.read(&correct_byte, sizeof(char))) {
      // Si hay alguna diferencia, da un error y termina
      if (correct_byte != result_byte) {
        return testing::AssertionFailure() << "The files are different";
      }
    }
    return testing::AssertionSuccess();
  }

  // Test con archivo de np < 0.
  TEST_F(FileIOTestClassDeathTest, npNegative) {
    ASSERT_EXIT(Fluid::FileIO("NpNegative.fld", "out.fld"),
                testing::ExitedWithCode((unsigned char) -5),
                "Error: Invalid number of particles: " + std::to_string(Const::np_negative) + "\n");
  }

  // Test con archivo de np == 0.
  TEST_F(FileIOTestClassDeathTest, npEqualZero) {
    ASSERT_EXIT(Fluid::FileIO("NpEqualZero.fld", "out.fld"),
                testing::ExitedWithCode((unsigned char) -5),
                "Error: Invalid number of particles: " + std::to_string(Const::np_zero) + "\n");
  }

  // Test con archivo de np positivo y mismo número de partículas.
  TEST_F(FileIOTestClass, npTrue) {
    Fluid::FileIO iotest{"NpTrue.fld", "out.fld"};
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    // Redirige std::cout a un buffer para poder leerlo y comprobar que es correcto
    std::stringstream buffer;
    std::streambuf * sbuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());
    // Ejecuta la prueba
    iotest.load(testGrid);
    // Restaura std::cout
    std::cout.rdbuf(sbuf);
    // Crea la salida esperada y la compara
    std::stringstream expected;
    expected.setf(std::ios::fixed);
    expected.precision(DoubleFormatPrecision);
    expected << "Number of particles: " << Const::it_np_true
             << "\nParticles per meter: " << Const::ppm << "\nSmoothing length: " << testGrid.h()
             << "\nParticle mass: " << testGrid.mass() << "\nGrid size: " << testGrid.count().x
             << " x " << testGrid.count().y << " x " << testGrid.count().z << "\nNumber of blocks: "
             << testGrid.count().x * testGrid.count().y * testGrid.count().z
             << "\nBlock size: " << testGrid.sizes().x << " x " << testGrid.sizes().y << " x "
             << testGrid.sizes().z << "\n";
    EXPECT_EQ(buffer.str(), expected.str());
    iotest.dump(testGrid);
    EXPECT_TRUE(compare_files("NpTrue.fld", "out.fld"));
  }

  // Test con archivo de np positivo y menor número de partículas que las requeridas.
  TEST_F(FileIOTestClassDeathTest, npLowerFalse) {
    Fluid::FileIO iotest{"NpLowerFalse.fld", "out.fld"};
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    ASSERT_EXIT(
        iotest.load(testGrid), testing::ExitedWithCode((unsigned char) -5),
        "Error: Number of particles mismatch. Header: " + std::to_string(Const::np_lower_false) +
            " Found: " + std::to_string(Const::it_np_lower_false) + "\n");
  }

  // Test con archivo de np positivo y mayor número de particulas de las requeridas.
  TEST_F(FileIOTestClassDeathTest, npGraterFalse) {
    Fluid::FileIO iotest{"NpGreaterFalse.fld", "out.fld"};
    Fluid::Grid testGrid{Const::np_greater_false, Const::ppm};

    ASSERT_EXIT(
        iotest.load(testGrid), testing::ExitedWithCode((unsigned char) -5),
        "Error: Number of particles mismatch. Header: " + std::to_string(Const::np_greater_false) +
            " Found: " + std::to_string(Const::it_np_greater_false) + "\n");
  }

  // Test con archivo de datos no válido. (Archivo no existe).
  TEST_F(FileIOTestClassDeathTest, BadInputFile) {
    ASSERT_EXIT(Fluid::FileIO("entrada.fld", "out.fld"),
                testing::ExitedWithCode((unsigned char) -3),
                "Error: Cannot open entrada.fld for reading");
  }

  // Test con archivo de salida de solo lectura.
  TEST_F(FileIOTestClassDeathTest, BadOutputFile) {
    ASSERT_EXIT(Fluid::FileIO("NpTrue.fld", "r.fld"), testing::ExitedWithCode((unsigned char) -4),
                "Error: Cannot open r.fld for writing");
  }
}  // namespace FileIOTest
