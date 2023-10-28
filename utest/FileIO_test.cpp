// funcion main customizada para tests

#include "../sim/fileio.hpp"
#include "../sim/grid.hpp"
#include "FileIO_test.hpp"
#include <gtest/gtest.h>

namespace FileIOTest {

TEST(FileIOTestClass, npNegative) {

    Fluid::FileIO iotest1{};
    iotest1.open("NpNegative.fld", "out.fld");

    ASSERT_EQ(iotest1.error(), -5);
}

TEST(FileIOTestClass, npEqualZero) {
    Fluid::FileIO iotest{};
    iotest.open("NpEqualZero.fld", "out.fld");
    ASSERT_EQ(iotest.error(), -5);
}

TEST(FileIOTestClass, npTrue) {
    Fluid::FileIO iotest{};
    iotest.open("NpTrue.fld", "out.fld");
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    iotest.load(testGrid);
    ASSERT_EQ(iotest, true);
}

TEST(FileIOTestClass, npLowerFalse) {
    Fluid::FileIO iotest{};
    iotest.open("NpLowerFalse.fld", "out.fld");
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    iotest.load(testGrid);
    ASSERT_EQ(iotest.error(), -5);
}

TEST(FileIOTestClass, npGraterFalse) {
    Fluid::FileIO iotest{};
    iotest.open("NpGraterFalse.fld", "out.fld");
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    iotest.load(testGrid);
    ASSERT_EQ(iotest.error(), -5);
}

TEST(FileIOTestClass, BadInputFile) {
    Fluid::FileIO iotest{};
    iotest.open("entrada.fld", "out.fld");
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    iotest.load(testGrid);
    ASSERT_EQ(iotest.error(), -3);
}

TEST(FileIOTestClass, BadOutputFile) {
    Fluid::FileIO iotest{};
    iotest.open("NpTrue.fld", "salida.fld");
    Fluid::Grid testGrid{iotest.np(), iotest.ppm()};
    iotest.load(testGrid);
    ASSERT_EQ(iotest.error(), -4);
}
} // namespace FileIOTest

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
