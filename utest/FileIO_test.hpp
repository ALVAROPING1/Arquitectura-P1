#pragma once

#include "../sim/io.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

namespace FileIOTest {

namespace Const {

const float ppm = 75;
const float np_negative = -50;
const float np_zero = 0;
const float np_true = 5;
const float np_lower_false = 5;
const float np_greater_false = 5;

const Fluid::Vector3<double> vel{1, 1, 1};
const Fluid::Vector3<double> pos{1, 1, 1};
const Fluid::Vector3<double> hvel{1, 1, 1};

const int it_np_true = 5;
const int it_np_lower_false = 3;
const int it_np_greater_false = 10;

}; // namespace Const

class FileIOTestClass : public testing::Test {

    static bool prepare_np_negative(const std::string &name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
            std::cout << "El archivo" << name << " no se ha podido crear\n";
            return false;
        }

        Fluid::IO::write<float>(file, Const::ppm);
        Fluid::IO::write<float>(file, Const::np_negative);

        file.close();
        return true;
    };

    static bool prepare_np_zero(const std::string &name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
            std::cout << "El archivo" << name << " no se ha podido crear\n";
            return false;
        }

        Fluid::IO::write<float>(file, Const::ppm);
        Fluid::IO::write<float>(file, Const::np_zero);

        file.close();
        return true;
    };
    static bool prepare_np_true(const std::string &name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
            std::cout << "El archivo" << name << " no se ha podido crear\n";
            return false;
        }

        Fluid::IO::write<float>(file, Const::ppm);
        Fluid::IO::write<float>(file, Const::np_true);

        for (int i = 0; i < Const::it_np_true; i++) {
            Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        file.close();
        return true;
    };

    static bool prepare_np_lower_false(const std::string &name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
            std::cout << "El archivo" << name << " no se ha podido crear\n";
            return false;
        }

        Fluid::IO::write<float>(file, Const::ppm);
        Fluid::IO::write<float>(file, Const::np_lower_false);

        for (int i = 0; i < Const::it_np_lower_false; i++) {
            Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        file.close();
        return true;
    };

    static bool prepare_np_greater_false(const std::string &name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
            std::cout << "El archivo" << name << " no se ha podido crear\n";
            return false;
        }

        Fluid::IO::write<float>(file, Const::ppm);
        Fluid::IO::write<float>(file, Const::np_greater_false);

        for (int i = 0; i < Const::it_np_greater_false; i++) {
            Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        file.close();
        return true;
    };

    static bool prepare_out(const std::string &name) {
        std::ofstream file{name, std::ios::binary};
        if (!file) {
            std::cout << "El archivo de salida no se pudo crear\n";
            return false;
        }

        file.close();
        return true;
    };

    void SetUp() override {

        const bool result_1 = prepare_np_negative("NpNegative.fld");
        const bool result_2 = prepare_np_zero("NpEqualZero.fld");
        const bool result_3 = prepare_np_true("NpTrue.fld");
        const bool result_4 = prepare_np_lower_false("NpLowerFalse.fld");
        const bool result_5 = prepare_np_greater_false("NpGreaterFalse.fld");
        const bool result_out = prepare_out("out.fld");

        if (!result_1 || !result_2 || !result_3 || !result_4 || !result_5 ||
            !result_out) {
            std::cout << "Las pruebas no se prepararon correctamente\n";
        } else {
            std::cout << "Los archivos se crearon correctamente!!\n";
        };
    };
    void TearDown() override {

        const int rem1 = std::remove("NpNegative.fld");
        const int rem2 = std::remove("NpEqualZero.fld");
        const int rem3 = std::remove("NpTrue.fld");
        const int rem4 = std::remove("NpLowerFalse.fld");
        const int rem5 = std::remove("NpGreaterFalse.fld");
        const int remout = std::remove("out.fld");

        if (rem1 != 0 || rem2 != 0 || rem3 != 0 || rem4 != 0 || rem5 != 0 ||
            remout != 0) {
            std::cout
                << "Los archivos de tests FileIO no se han podido borrar!!\n";
        } else {
            std::cout << "Los archivos se borraron correctamente!!\n";
        };
    };
};

} // namespace FileIOTest
