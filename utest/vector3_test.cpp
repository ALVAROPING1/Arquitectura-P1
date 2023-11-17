#include "sim/vector3.hpp"

#include <gtest/gtest.h>

namespace Vector3Test {
  constexpr Fluid::Vector3<int> int_1{10, 15, 20};
  constexpr Fluid::Vector3<int> int_2{5, 10, 15};
  constexpr Fluid::Vector3<int> int_suma{15, 25, 35};
  constexpr Fluid::Vector3<int> int_resta{5, 5, 5};

  constexpr Fluid::Vector3<float> float_1{1.0, 0.5, 2.0};
  constexpr Fluid::Vector3<float> float_2{3.5, 1.0, 0.5};
  constexpr Fluid::Vector3<float> float_suma{4.5, 1.5, 2.5};
  constexpr Fluid::Vector3<float> float_resta{-2.5, -0.5, 1.5};

  constexpr Fluid::Vector3<double> double_1{1.0, 0.5, 2.0};
  constexpr Fluid::Vector3<double> double_2{3.5, 1.0, 0.5};
  constexpr Fluid::Vector3<double> double_suma{4.5, 1.5, 2.5};
  constexpr Fluid::Vector3<double> double_resta{-2.5, -0.5, 1.5};

  // pruebas de std::ostream <<
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, out_int_1) {
    std::stringstream out{};
    out << int_1;
    ASSERT_EQ(out.str(), "(x: 10, y: 15, z: 20)");
  }

  // floats
  TEST(Vector3Test, out_float_1) {
    std::stringstream out{};
    out << float_1;
    ASSERT_EQ(out.str(), "(x: 1, y: 0.5, z: 2)");
  }

  // doubles
  TEST(Vector3Test, out_double_1) {
    std::stringstream out{};
    out << double_1;
    ASSERT_EQ(out.str(), "(x: 1, y: 0.5, z: 2)");
  }

  // pruebas de suma
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, sumar_int_1) {
    ASSERT_EQ(int_1 + int_2, int_suma);
  }

  // floats
  TEST(Vector3Test, sumar_float_1) {
    ASSERT_EQ(float_1 + float_2, float_suma);
  }

  // doubles
  TEST(Vector3Test, sumar_double_1) {
    ASSERT_EQ(double_1 + double_2, double_suma);
  }

  // pruebas resta
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, restar_int_1) {
    ASSERT_EQ(int_1 - int_2, int_resta);
  }

  // floats
  TEST(Vector3Test, restar_float_1) {
    ASSERT_EQ(float_1 - float_2, float_resta);
  }

  // doubles
  TEST(Vector3Test, restar_double_1) {
    ASSERT_EQ(double_1 - double_2, double_resta);
  }

  // pruebas +=
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, suma_igual_int_1) {
    Fluid::Vector3<int> actual_answer  = int_1;
    ASSERT_EQ(actual_answer += int_2, int_suma);
    ASSERT_EQ(actual_answer, int_suma);
  }

  // floats
  TEST(Vector3Test, suma_igual_float_1) {
    Fluid::Vector3<float> actual_answer  = float_1;
    ASSERT_EQ(actual_answer += float_2, float_suma);
    ASSERT_EQ(actual_answer, float_suma);
  }

  // doubles
  TEST(Vector3Test, suma_igual_double_1) {
    Fluid::Vector3<double> actual_answer  = double_1;
    ASSERT_EQ(actual_answer += double_2, double_suma);
    ASSERT_EQ(actual_answer, double_suma);
  }

  // pruebas -=
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, resta_igual_int_1) {
    Fluid::Vector3<int> actual_answer  = int_1;
    ASSERT_EQ(actual_answer -= int_2, int_resta);
    ASSERT_EQ(actual_answer, int_resta);
  }

  // floats
  TEST(Vector3Test, resta_igual_float_1) {
    Fluid::Vector3<float> actual_answer  = float_1;
    ASSERT_EQ(actual_answer -= float_2, float_resta);
    ASSERT_EQ(actual_answer, float_resta);
  }

  // doubles
  TEST(Vector3Test, resta_igual_double_1) {
    Fluid::Vector3<double> actual_answer  = double_1;
    ASSERT_EQ(actual_answer -= double_2, double_resta);
    ASSERT_EQ(actual_answer, double_resta);
  }

  // prueaba vector*constante
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, multiplicar_constante_int_1) {
    Fluid::Vector3<int> constexpr expected_answer = {50, 75, 100};
    ASSERT_EQ(int_1 * 5, expected_answer);
  }

  // floats
  TEST(Vector3Test, multiplicar_constante_float_1) {
    Fluid::Vector3<float> constexpr expected_answer = {5.0, 2.5, 10.0};
    ASSERT_EQ(float_1 * 5, expected_answer);
    ASSERT_EQ(float_1 * 5.0F, expected_answer);
  }

  // doubles
  TEST(Vector3Test, multiplicar_constante_double_1) {
    Fluid::Vector3<double> constexpr expected_answer = {5.0, 2.5, 10.0};
    ASSERT_EQ(double_1 * 5, expected_answer);
    ASSERT_EQ(double_1 * 5.0F, expected_answer);
    ASSERT_EQ(double_1 * 5.0, expected_answer);
  }

  // prueba vector/constante
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, dividir_constante_int_1) {
    Fluid::Vector3<int> constexpr expected_answer = {2, 3, 4};
    ASSERT_EQ(int_1 / 5, expected_answer);
  }

  // floats
  TEST(Vector3Test, dividir_constante_float_1) {
    Fluid::Vector3<float> constexpr expected_answer = {0.5, 0.25, 1.0};
    ASSERT_EQ(float_1 / 2, expected_answer);
    ASSERT_EQ(float_1 / 2.0F, expected_answer);
  }

  // doubles
  TEST(Vector3Test, dividir_constante_double_1) {
    Fluid::Vector3<double> constexpr expected_answer = {0.5, 0.25, 1.0};
    ASSERT_EQ(double_1 / 2, expected_answer);
    ASSERT_EQ(double_1 / 2.0F, expected_answer);
    ASSERT_EQ(double_1 / 2.0, expected_answer);
  }

  // prueba vector/vector
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, dividir_vector_int_1) {
    Fluid::Vector3<int> constexpr expected_answer = {2, 1, 1};
    ASSERT_EQ(int_1 / int_2, expected_answer);
  }

  // floats
  TEST(Vector3Test, dividir_vector_float_1) {
    Fluid::Vector3<float> constexpr expected_answer_1 = {1.0 / 3.5, 0.5, 4.0};
    ASSERT_EQ(float_1 / float_2, expected_answer_1);
  }

  // doubles
  TEST(Vector3Test, dividir_vector_double_1) {
    Fluid::Vector3<double> constexpr expected_answer = {1.0 / 3.5, 0.5, 4.0};
    ASSERT_EQ(double_1 / double_2, expected_answer);
  }

  // Static cast
  // clases de equivalencia
  // floats a doubles
  TEST(Vector3Test, static_cast) {
    ASSERT_EQ(static_cast<Fluid::Vector3<double>>(float_1), double_1);
  }

  // square norm
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, square_norm_int_1) {
    int constexpr expected_answer = 10 * 10 + 15 * 15 + 20 * 20;
    ASSERT_EQ(int_1.square_norm(), expected_answer);
  }

  // floats
  TEST(Vector3Test, square_norm_float_1) {
    float constexpr expected_answer = 1.0 * 1.0 + 0.5 * 0.5 + 2.0 * 2.0;
    ASSERT_EQ(float_1.square_norm(), expected_answer);
  }

  // doubles
  TEST(Vector3Test, square_norm_double_1) {
    float constexpr expected_answer = 1.0 * 1.0 + 0.5 * 0.5 + 2.0 * 2.0;
    ASSERT_EQ(double_1.square_norm(), expected_answer);
  }

  // clamp
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, clamp_int_1) {
    Fluid::Vector3<int> constexpr low             = {11, 9, 14};
    Fluid::Vector3<int> constexpr high            = {15, 11, 28};
    Fluid::Vector3<int> constexpr expected_answer = {11, 11, 20};
    ASSERT_EQ(int_1.clamp(low, high), expected_answer);
  }

  // floats
  TEST(Vector3Test, clamp_float_1) {
    Fluid::Vector3<float> constexpr low             = {0.0, 0.4, 3.0};
    Fluid::Vector3<float> constexpr high            = {0.5, 0.6, 4.5};
    Fluid::Vector3<float> constexpr expected_answer = {0.5, 0.5, 3.0};
    ASSERT_EQ(float_1.clamp(low, high), expected_answer);
  }

  // doubles
  TEST(Vector3Test, clamp_double_1) {
    Fluid::Vector3<double> constexpr low             = {0.0, 0.4, 3.0};
    Fluid::Vector3<double> constexpr high            = {0.5, 0.6, 4.5};
    Fluid::Vector3<double> constexpr expected_answer = {0.5, 0.5, 3.0};
    ASSERT_EQ(double_1.clamp(low, high), expected_answer);
  }

  // vector == vector
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, igual_que_int_1) {
    ASSERT_EQ(int_1 == int_1, true);
    ASSERT_EQ(int_1 == int_2, false);
  }

  // floats
  TEST(Vector3Test, igual_que_float_1) {
    ASSERT_EQ(float_1 == float_1, true);
    ASSERT_EQ(float_1 == float_2, false);
  }

  // doubles
  TEST(Vector3Test, igual_que_double_1) {
    ASSERT_EQ(double_1 == double_1, true);
    ASSERT_EQ(double_1 == double_2, false);
  }

  // vector.get<W>()
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, get_W_int_1) {
    ASSERT_EQ(int_1.get<Fluid::Axis::x>(), int_1.x);
    ASSERT_EQ(int_1.get<Fluid::Axis::y>(), int_1.y);
    ASSERT_EQ(int_1.get<Fluid::Axis::z>(), int_1.z);
  }

  // floats
  TEST(Vector3Test, get_W_float_1) {
    ASSERT_EQ(float_1.get<Fluid::Axis::x>(), float_1.x);
    ASSERT_EQ(float_1.get<Fluid::Axis::y>(), float_1.y);
    ASSERT_EQ(float_1.get<Fluid::Axis::z>(), float_1.z);
  }

  // doubles
  TEST(Vector3Test, get_W_double_1) {
    ASSERT_EQ(double_1.get<Fluid::Axis::x>(), double_1.x);
    ASSERT_EQ(double_1.get<Fluid::Axis::y>(), double_1.y);
    ASSERT_EQ(double_1.get<Fluid::Axis::z>(), double_1.z);
  }

  // vector.set<W>()
  // clases de equivalencia
  // enteros
  TEST(Vector3Test, set_W_int_1) {
    Fluid::Vector3<int> actual_answer;
    actual_answer.set<Fluid::Axis::x>() = int_1.x;
    actual_answer.set<Fluid::Axis::y>() = int_1.y;
    actual_answer.set<Fluid::Axis::z>() = int_1.z;
    ASSERT_EQ(actual_answer, int_1);
  }

  // floats
  TEST(Vector3Test, set_W_float_1) {
    Fluid::Vector3<float> actual_answer;
    actual_answer.set<Fluid::Axis::x>() = float_1.x;
    actual_answer.set<Fluid::Axis::y>() = float_1.y;
    actual_answer.set<Fluid::Axis::z>() = float_1.z;
    ASSERT_EQ(actual_answer, float_1);
  }

  // doubles
  TEST(Vector3Test, set_W_double_1) {
    Fluid::Vector3<double> actual_answer;
    actual_answer.set<Fluid::Axis::x>() = double_1.x;
    actual_answer.set<Fluid::Axis::y>() = double_1.y;
    actual_answer.set<Fluid::Axis::z>() = double_1.z;
    ASSERT_EQ(actual_answer, double_1);
  }

}  // namespace Vector3Test
