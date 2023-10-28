#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

namespace FileIOTest {

class FileIOTestClass : public testing::Test {

    int ppm = 75;
    int np1 = -50, np2 = 0, np3 = 5, np4 = 5, np5 = 5;

    FileIOTestClass() = default;
    void SetUp() override {
        std::ofstream archivo_1{"NpNegative.fld"};
        std::ofstream archivo_2{"NpEqualZero.fld"};
        std::ofstream archivo_3{"NpTrue.fld"};
        std::ofstream archivo_4{"NpLowerFalse.fld"};
        std::ofstream archivo_5{"NpGreaterFalse.fld"};
        std::ofstream archivo_out{"out.fld"};

        if (!archivo_1 || !archivo_2 || !archivo_3 || !archivo_4 ||
            !archivo_5) {
            std::cout
                << "Los archivos de tests FileIO no se han podido crear!!\n";
            return;
        }

        archivo_1 << ppm;
        archivo_1 << np1;

        archivo_2 << ppm;
        archivo_2 << np2;

        archivo_3 << ppm;
        archivo_3 << np3;

        for (int i = 0; i < 5; i++) {
            archivo_3 << i << i << i << i << i << i << 1 << 2 << 3;
        }

        archivo_4 << ppm;
        archivo_4 << np4;

        for (int i = 0; i < 3; i++) {
            archivo_4 << i << i << i << i << i << i << 1 << 2 << 3;
        }

        archivo_5 << ppm;
        archivo_5 << np5;

        for (int i = 0; i < 10; i++) {
            archivo_4 << i << i << i << i << i << i << 1 << 2 << 3;
        }
    };
    void TearDown() override {

        int s1 = std::remove("NpNegative.fld");
        int s2 = std::remove("NpEqualZero.fld");
        int s3 = std::remove("NpTrue.fld");
        int s4 = std::remove("NpLowerFalse.fld");
        int s5 = std::remove("NpGreaterFalse.fld");
        int sout = std::remove("out.fld");

        if (s1 != 0 || s2 != 0 || s3 != 0 || s4 != 0 || s5 != 0 || sout != 0) {
            std::cout
                << "Los archivos de tests FileIO no se han podido borrar!!\n";
        }
    };
};

} // namespace FileIOTest
