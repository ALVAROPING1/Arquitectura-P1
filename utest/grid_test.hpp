#pragma once

#include "sim/vector3.hpp"

#include <gtest/gtest.h>

namespace GridTest {

  // Espacio virtual de constantes para la creación de archivos custom.
  // Incluye constantes para el número de partículas por metro y para los diferentes números de
  // particulas a inyectar en los archivos custom. También, contiene unos vectores de la clase de
  // "Fluid::Vector3<float>" que representan la posición, el gradiente y la velocidad de cada
  // partícula.
  namespace Const {
    // constantes de pruebas.
    float constexpr ppm = 75;
    int constexpr num   = 5;

    // vectores de ejemplo para las partículas de prueba.
    Fluid::Vector3<float> constexpr vel{1, 1, 1};
    Fluid::Vector3<float> constexpr pos{0, 0, 0};
    Fluid::Vector3<float> constexpr hvel{1, 1, 1};

    // vectores de posición fuera del recinto
    Fluid::Vector3<float> constexpr pos_inf_x{-1, 0, 0};
    Fluid::Vector3<float> constexpr pos_inf_y{0, -1, 0};
    Fluid::Vector3<float> constexpr pos_inf_z{0, 0, -1};
    Fluid::Vector3<float> constexpr pos_sup_x{1, 0, 0};
    Fluid::Vector3<float> constexpr pos_sup_y{0, 1, 0};
    Fluid::Vector3<float> constexpr pos_sup_z{0, 0, 1};

  };  // namespace Const

  class GridTestClass : public testing::Test {
      // Funcíon que se ejecuta antes de cada prueba para preparar el entorno.
      void SetUp() override{};

      // Función que destruye el entorno para pruebas.
      void TearDown() override{};
  };

}  // namespace GridTest
