// Pruebas unitarias para progargs
//
#include "../sim/argumentparser.hpp"
#include "../sim/io.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>
#include <span>
namespace ProgArgsTest { // No se incluye las pruebas de testeo de los archivos
                         // ya que se indicaron en las pruebas unitarias de
                         // FileIO.
const int items_more_args = 6;
const int items_args = 4;
const int items_less_args = 3;

void prepare_file(std::string const &name) {
    std::ofstream file{name, std::ios::binary};
    const float ppm = 75;
    const float np_i = 1;
    const Fluid::Vector3<float> h_i{1, 1, 1};

    Fluid::IO::write<float>(file, ppm);
    Fluid::IO::write<float>(file, np_i);

    Fluid::IO::write(file, h_i, h_i, h_i);
    file.close();
}

void close_file(const char *name) {

    const int rem1 = std::remove(name);

    if (rem1 != 0) {
        std::cout
            << "El archivo de tests ProgArgsTest no se ha podido borrar!!\n";
    } else {
        std::cout << "El archivo se borro correctamente!!\n";
    };
}

TEST(ProgArgsTestClass, OK) {

    prepare_file("a.fld");
    // Lista de argumentos para el test de ok.

    std::array<gsl::czstring, items_args> ok_v = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("a.fld"),
        static_cast<gsl::czstring>("out.fld")};
    const std::span<gsl::czstring> ok_s{ok_v};

    const Fluid::ArgumentParser parser{ok_s};
    ASSERT_EQ(parser.error(), 0);
    close_file("a.fld");

} // Comparar error con 0.

TEST(ProgArgsTestClass, LessArgs) {
    // Lista de argumentos para el test con menos argumentos.
    std::array<gsl::czstring, items_less_args> less_v = {
        static_cast<gsl::czstring>("fluid"),
        static_cast<gsl::czstring>("init.fld"),
        static_cast<gsl::czstring>("final.fld")};
    const std::span<gsl::czstring> lessArgs{less_v};

    const Fluid::ArgumentParser parser{lessArgs};
    ASSERT_EQ(parser.error(), -1);

} // Comparar error con -1.

TEST(ProgArgsTestClass, MoreArgs) {
    // Lista de argumentos para el test con mas argumentos.
    std::array<gsl::czstring, items_more_args> more_v = {
        static_cast<gsl::czstring>("fluid"),
        static_cast<gsl::czstring>("1000"),
        static_cast<gsl::czstring>("init.fld"),
        static_cast<gsl::czstring>("final.fld"),
        static_cast<gsl::czstring>("60"),
        static_cast<gsl::czstring>("hola")};
    const std::span<gsl::czstring> moreArgs{more_v};

    const Fluid::ArgumentParser parser{moreArgs};
    ASSERT_EQ(parser.error(), -1);

} // Comparar error con -1.

TEST(ProgArgsTestClass, InvIterations) {
    // Lista de argumentos para el test con numero de iteraciones no numericas.
    std::array<gsl::czstring, items_args> inv_v = {
        static_cast<gsl::czstring>("fluid"),
        static_cast<gsl::czstring>("byebye"),
        static_cast<gsl::czstring>("init.fld"),
        static_cast<gsl::czstring>("final.fld")};
    const std::span<gsl::czstring> invIterations{inv_v};

    const Fluid::ArgumentParser parser{invIterations};
    ASSERT_EQ(parser.error(), -1);

} // Parametro de iteraciones no digitos. Comparar error con -1.

TEST(ProgArgsTestClass, NegIterations) {
    // Lista de argumentos para el test con numero de iterationes negativo
    std::array<gsl::czstring, items_args> neg_v = {
        static_cast<gsl::czstring>("fluid"), static_cast<gsl::czstring>("-501"),
        static_cast<gsl::czstring>("init.fld"),
        static_cast<gsl::czstring>("final.fld")};
    const std::span<gsl::czstring> negIterations{neg_v};

    const Fluid::ArgumentParser parser{negIterations};
    ASSERT_EQ(parser.error(), -2);

} // Iteraciones negativas. Comparar error con -2.

}; // namespace ProgArgsTest

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
