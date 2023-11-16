#pragma once

#define FLUID_DEBUG
#include "sim/io.hpp"

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

    // vectores de ejemplo para las partículas de prueba.
    Fluid::Vector3<float> const vel{1, 1, 1};
    Fluid::Vector3<float> const pos{1, 1, 1};
    Fluid::Vector3<float> const hvel{1, 1, 1};

    int const num = 5;
  };  // namespace Const

  class GridTestClass : public testing::Test {
      // Funcíon que se ejecuta antes de cada prueba para preparar el entorno.
      void SetUp() override{};

      // Función que destruye el entorno para pruebas.
      void TearDown() override{};
  };

}  // namespace GridTest
