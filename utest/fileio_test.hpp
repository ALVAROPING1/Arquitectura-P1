#pragma once

#include "sim/io.hpp"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

namespace FileIOTest {

  // Espacio virtual de constantes para la creación de archivos custom.
  // Incluye constantes para el número de partículas por metro y para los diferentes números de
  // particulas a inyectar en los archivos custom. También, contiene unos vectores de la clase de
  // "Fluid::Vector3<float>" que representan la posición, el gradiente y la velocidad de cada
  // partícula.
  namespace Const {
    // constantes de pruebas.
    float constexpr ppm            = 75;
    int constexpr np_negative      = -1;
    int constexpr np_zero          = 0;
    int constexpr np_true          = 1;
    int constexpr np_lower_false   = 5;
    int constexpr np_greater_false = 5;

    // vectores de ejemplo para las partículas de prueba.
    Fluid::Vector3<float> constexpr vel{1, 1, 1};
    Fluid::Vector3<float> constexpr pos{1, 1, 1};
    Fluid::Vector3<float> constexpr hvel{1, 1, 1};
  };  // namespace Const

  // Clase utilizada como preparatoria para los archivos utilizados en los tests.
  // También tene las funciones TearDown y SetUp que configuran el entorno antes y después de cada
  // test. Esta clase utiliza las funciones de el header "sim/io.hpp" para escribir los parámetros
  // en los archivos. En cada función preparatoria, se comprueba que el archivo se ha creado
  // correctamente. Sino, se notifica de que los archivos no se han creado.
  class FileIOTestClass : public testing::Test {
      static std::ofstream prepare_file(std::string const & name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
          std::cout << "El archivo" << name << " no se ha podido crear\n";
        } else {
          Fluid::IO::write(file, Const::ppm);
        }

        return file;
      }

      // creación de un archivo con un número de particulas negativas.
      static bool prepare_np_negative(std::string const & name) {
        std::ofstream file = prepare_file(name);
        if (!file) { return false; }
        Fluid::IO::write(file, Const::np_negative);
        return true;
      };

      // creación de un archivo con indicador de número de paratículas nulo.
      static bool prepare_np_zero(std::string const & name) {
        std::ofstream file = prepare_file(name);
        if (!file) { return false; }
        Fluid::IO::write(file, Const::np_zero);

        return true;
      };

      // creación de un archivo con número de partículas positivo y coincide con las partículas
      // introducidas en el propio archivo.
      static bool prepare_np_true(std::string const & name) {
        std::ofstream file = prepare_file(name);
        if (!file) { return false; }
        Fluid::IO::write(file, Const::np_true);

        for (int i = 0; i < Const::np_true; i++) {
          Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        return true;
      };

      // creación de un archivo con número de partículas positivo pero hay menos particulas de las
      // indicadas.
      static bool prepare_np_lower_false(std::string const & name) {
        std::ofstream file = prepare_file(name);
        if (!file) { return false; }
        Fluid::IO::write(file, Const::np_lower_false);

        for (int i = 0; i < Const::np_lower_false - 1; i++) {
          Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        return true;
      };

      // creación de un archivo con número de partículas positivo pero hay mas particulas de las
      // indicadas.
      static bool prepare_np_greater_false(std::string const & name) {
        std::ofstream file = prepare_file(name);
        if (!file) { return false; }
        Fluid::IO::write(file, Const::np_greater_false);

        for (int i = 0; i < Const::np_greater_false + 1; i++) {
          Fluid::IO::write(file, Const::pos, Const::hvel, Const::vel);
        }

        return true;
      };

      // creación de un archivo en binario de salida para las pruebas.
      static bool prepare_out(std::string const & name) {
        std::ofstream const file{name, std::ios::binary};
        if (!file) {
          std::cout << "El archivo de salida no se pudo crear\n";
          return false;
        }

        return true;
      };

      // creación de un archivo de salida que solo se puede leer para las pruebas.
      static bool prepare_only_read_out(std::string const & name) {
        std::ofstream file{name, std::ios::binary};

        if (!file) {
          std::cout << "El archivo de solo lectura no se puede crear\n";
          return false;
        }
        file.close();

        if (chmod(name.c_str(), 0) != 0) {
          std::cout << "No se pudieron cambiar los permisos de el archivo de "
                       "solo lectura\n";
          return false;
        }
        return true;
      }

      // Funcíon que se ejecuta antes de cada prueba para preparar el entorno.
      void SetUp() override {
        bool const result_1        = prepare_np_negative("NpNegative.fld");
        bool const result_2        = prepare_np_zero("NpEqualZero.fld");
        bool const result_3        = prepare_np_true("NpTrue.fld");
        bool const result_4        = prepare_np_lower_false("NpLowerFalse.fld");
        bool const result_5        = prepare_np_greater_false("NpGreaterFalse.fld");
        bool const result_out      = prepare_out("out.fld");
        bool const result_read_out = prepare_only_read_out("r.fld");

        if (!result_1 || !result_2 || !result_3 || !result_4 || !result_5 || !result_out ||
            !result_read_out) {
          std::cout << "Las pruebas no se prepararon correctamente: \n";
        }
      };

      // Función que borra los archivos después de cada prueba y destruye el entorno para pruebas.
      void TearDown() override {
        int const rem1       = std::remove("NpNegative.fld");
        int const rem2       = std::remove("NpEqualZero.fld");
        int const rem3       = std::remove("NpTrue.fld");
        int const rem4       = std::remove("NpLowerFalse.fld");
        int const rem5       = std::remove("NpGreaterFalse.fld");
        int const remout     = std::remove("out.fld");
        int const remlectout = std::remove("r.fld");

        if (rem1 != 0 || rem2 != 0 || rem3 != 0 || rem4 != 0 || rem5 != 0 || remout != 0 ||
            remlectout != 0) {
          std::cout << "Los archivos de tests FileIO no se han podido borrar!!\n";
        }
      };
  };

}  // namespace FileIOTest
