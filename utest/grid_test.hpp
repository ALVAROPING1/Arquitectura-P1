#pragma once

#define FLUID_DEBUG
#include "../sim/io.hpp"

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sys/stat.h>

namespace GridTest {

  // Espacio virtual de constantes para la creación de archivos custom.
  // Incluye constantes para el número de partículas por metro y para los diferentes números de
  // particulas a inyectar en los archivos custom. También, contiene unos vectores de la clase de
  // "Fluid::Vector3<float>" que representan la posición, el gradiente y la velocidad de cada
  // partícula.
  namespace Const {
    // constantes de pruebas.
    float const ppm = 75;
    int const f_np  = 5;

    // vectores de ejemplo para las partículas de prueba.
    Fluid::Vector3<float> const vel{1, 1, 1};
    Fluid::Vector3<float> const pos{1, 1, 1};
    Fluid::Vector3<float> const hvel{1, 1, 1};

    // iteraciones a realizar para agregar particulas de cada prueba.
    int const it_np_true = 5;
  };  // namespace Const

  class GridTestClass : public testing::Test {
      // Método que crea un archivo para asegurar la buena importación de los datos a grid.
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

      // Funcíon que se ejecuta antes de cada prueba para preparar el entorno.
      void SetUp() override {
        bool const result_3 = prepare_np_true("NpTrue.fld");

        if (!result_3) {
          std::cout << "Las pruebas no se prepararon correctamente: \n";
        } else {
          std::cout << "Los archivos se crearon correctamente!!\n";
        };
      };

      // Función que borra los archivos después de cada prueba y destruye el entorno para pruebas.
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
