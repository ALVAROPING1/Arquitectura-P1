#include "vector3_test.hpp"

#include "../sim/vector3.hpp"

#include <gtest/gtest.h>

namespace Vector3Test {
  // pruebas de suma
  // clases de equivalencia
  TEST_F(Vector3TestClass, sumar_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const expected_answer = {17, 46, 41};
    ASSERT_EQ(get_int_vector_1() + get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, sumar_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const expected_answer = {4.5, 1.5, 2.5};
    ASSERT_EQ(get_float_vector_1() + get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, sumar_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const expected_answer = {4.5, 1.5, 2.5};
    ASSERT_EQ(get_double_vector_1() + get_double_vector_2(), expected_answer);
  }

  // pruebas resta
  // clases de equivalencia
  TEST_F(Vector3TestClass, restar_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const expected_answer = {3, -22, 5};
    ASSERT_EQ(get_int_vector_1() - get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, restar_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const expected_answer = {-2.5, -0.5, 1.5};
    ASSERT_EQ(get_float_vector_1() - get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, restar_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const expected_answer = {-2.5, -0.5, 1.5};
    ASSERT_EQ(get_double_vector_1() - get_double_vector_2(), expected_answer);
  }

  // pruebas +=
  // clases de equivalencia
  TEST_F(Vector3TestClass, suma_igual_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const expected_answer = {10, 12, 23};
    Fluid::Vector3<int> actual_answer;
    actual_answer += get_int_vector_1();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  TEST_F(Vector3TestClass, suma_igual_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const expected_answer = {1.0, 0.5, 2.0};
    Fluid::Vector3<float> actual_answer;
    actual_answer += get_float_vector_1();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  TEST_F(Vector3TestClass, suma_igual_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const expected_answer = {3.5, 1.0, 0.5};
    Fluid::Vector3<double> actual_answer;
    actual_answer += get_double_vector_2();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  // pruebas -=
  // clases de equivalencia
  TEST_F(Vector3TestClass, resta_igual_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const expected_answer = {-7, -34, -18};
    Fluid::Vector3<int> actual_answer;
    actual_answer -= get_int_vector_2();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  TEST_F(Vector3TestClass, resta_igual_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const expected_answer = {-1.0, -0.5, -2.0};
    Fluid::Vector3<float> actual_answer;
    actual_answer -= get_float_vector_1();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  TEST_F(Vector3TestClass, resta_igual_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const expected_answer = {-3.5, -1.0, -0.5};
    Fluid::Vector3<double> actual_answer;
    actual_answer -= get_double_vector_2();
    ASSERT_EQ(actual_answer, expected_answer);
  }

  // prueaba vector*constante
  // clases de equivalencia
  TEST_F(Vector3TestClass, multiplicar_constante_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const expected_answer = {50, 60, 115};
    ASSERT_EQ(get_int_vector_1() * 5, expected_answer);
  }

  TEST_F(Vector3TestClass, multiplicar_constante_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const expected_answer = {5.0, 2.5, 10.0};
    ASSERT_EQ(get_float_vector_1() * 5, expected_answer);
  }

  TEST_F(Vector3TestClass, multiplicar_constante_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const expected_answer = {17.5, 5.0, 2.5};
    ASSERT_EQ(get_double_vector_2() * 5, expected_answer);
  }

  // prueba vector/constante
  // clases de equivalencia
  TEST_F(Vector3TestClass, dividir_constante_int_1) {
    Vector3TestClass::test_int_dividir_1();
    Fluid::Vector3<int> const expected_answer = {3, 11, 12};
    ASSERT_EQ(get_int_vector_1() / 11, expected_answer);
  }

  TEST_F(Vector3TestClass, dividir_constante_float_1) {
    Vector3TestClass::test_float_dividir_1();
    Fluid::Vector3<float> const expected_answer = {6.0, 12.0, 5.0};
    ASSERT_EQ(get_float_vector_1() / 7, expected_answer);
  }

  TEST_F(Vector3TestClass, dividir_constante_double_1) {
    Vector3TestClass::test_double_dividir_1();
    Fluid::Vector3<double> const expected_answer = {9.0, 4.0, 5.0};
    ASSERT_EQ(get_double_vector_1() / 9, expected_answer);
  }

  // prueba vector/vector
  // clases de equivalencia
  TEST_F(Vector3TestClass, dividir_vector_int_1) {
    Vector3TestClass::test_int_dividir_1();
    Fluid::Vector3<int> const expected_answer = {11, 11, 66};
    ASSERT_EQ(get_int_vector_1() / get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, dividir_vector_float_1) {
    Vector3TestClass::test_float_dividir_1();
    Fluid::Vector3<float> const expected_answer = {6.0, 7.0, 70.0};
    ASSERT_EQ(get_float_vector_1() / get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, dividir_vector_double_1) {
    Vector3TestClass::test_double_dividir_1();
    Fluid::Vector3<double> const expected_answer = {27.0, 18.0, 9.0};
    ASSERT_EQ(get_double_vector_1() / get_double_vector_2(), expected_answer);
  }

  // square norm
  // clases de equivalencia
  TEST_F(Vector3TestClass, square_norm_int_1) {
    Vector3TestClass::test_int_sumar_1();
    int const expected_answer = 1529;
    ASSERT_EQ(get_int_vector_2().square_norm(), expected_answer);
  }

  TEST_F(Vector3TestClass, square_norm_float_1) {
    Vector3TestClass::test_float_sumar_1();
    float const expected_answer = 5.25;
    ASSERT_EQ(get_float_vector_1().square_norm(), expected_answer);
  }

  TEST_F(Vector3TestClass, square_norm_double_1) {
    Vector3TestClass::test_double_sumar_1();
    float const expected_answer = 13.5;
    ASSERT_EQ(get_double_vector_2().square_norm(), expected_answer);
  }

  // clamp
  // clases de equivalencia
  TEST_F(Vector3TestClass, clamp_int_1) {
    Vector3TestClass::test_int_sumar_1();
    Fluid::Vector3<int> const low             = {11, 9, 24};
    Fluid::Vector3<int> const high            = {15, 11, 28};
    Fluid::Vector3<int> const expected_answer = {11, 11, 24};
    ASSERT_EQ(get_int_vector_1().clamp(low, high), expected_answer);
  }

  TEST_F(Vector3TestClass, clamp_float_1) {
    Vector3TestClass::test_float_sumar_1();
    Fluid::Vector3<float> const low             = {3.6, 0.4, 0.7};
    Fluid::Vector3<float> const high            = {3.8, 0.6, 0.8};
    Fluid::Vector3<float> const expected_answer = {3.6, 0.6, 0.7};
    ASSERT_EQ(get_float_vector_2().clamp(low, high), expected_answer);
  }

  TEST_F(Vector3TestClass, clamp_double_1) {
    Vector3TestClass::test_double_sumar_1();
    Fluid::Vector3<double> const low             = {3.6, 0.4, 0.7};
    Fluid::Vector3<double> const high            = {3.8, 0.6, 0.8};
    Fluid::Vector3<double> const expected_answer = {3.6, 0.6, 0.7};
    ASSERT_EQ(get_double_vector_2().clamp(low, high), expected_answer);
  }

  // vector < vector
  // clases de equivalencia
  TEST_F(Vector3TestClass, menor_que_int_1) {
    Vector3TestClass::test_int_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_int_vector_1() < get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, menor_que_float_1) {
    Vector3TestClass::test_float_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_float_vector_1() < get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, menor_que_double_1) {
    Vector3TestClass::test_double_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_double_vector_1() < get_double_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, menor_que_int_2) {
    Vector3TestClass::test_menor_que_int_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_int_vector_1() < get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, menor_que_float_2) {
    Vector3TestClass::test_menor_que_float_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_float_vector_1() < get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, menor_que_double_2) {
    Vector3TestClass::test_menor_que_double_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_double_vector_1() < get_double_vector_2(), expected_answer);
  }

  // vector > vector
  // clases de equivalencia
  TEST_F(Vector3TestClass, mayor_que_int_1) {
    Vector3TestClass::test_int_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_int_vector_1() > get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, mayor_que_float_1) {
    Vector3TestClass::test_float_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_float_vector_1() > get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, mayor_que_double_1) {
    Vector3TestClass::test_double_sumar_1();
    bool const expected_answer = true;
    ASSERT_EQ(get_double_vector_1() > get_double_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, mayor_que_int_2) {
    Vector3TestClass::test_menor_que_int_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_int_vector_2() > get_int_vector_1(), expected_answer);
  }

  TEST_F(Vector3TestClass, mayor_que_float_2) {
    Vector3TestClass::test_menor_que_float_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_float_vector_2() > get_float_vector_1(), expected_answer);
  }

  TEST_F(Vector3TestClass, mayor_que_double_2) {
    Vector3TestClass::test_menor_que_double_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_double_vector_2() > get_double_vector_1(), expected_answer);
  }

  // vector == vector
  // clases de equivalencia
  TEST_F(Vector3TestClass, igual_que_int_1) {
    Vector3TestClass::test_igual_que_int_2();
    bool const expected_answer = true;
    ASSERT_EQ(get_int_vector_1() == get_int_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, igual_que_float_1) {
    Vector3TestClass::test_igual_que_float_2();
    bool const expected_answer = true;
    ASSERT_EQ(get_float_vector_1() == get_float_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, igual_que_double_1) {
    Vector3TestClass::test_igual_que_double_2();
    bool const expected_answer = true;
    ASSERT_EQ(get_double_vector_1() == get_double_vector_2(), expected_answer);
  }

  TEST_F(Vector3TestClass, igual_que_int_2) {
    Vector3TestClass::test_menor_que_int_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_int_vector_2() == get_int_vector_1(), expected_answer);
  }

  TEST_F(Vector3TestClass, igual_que_float_2) {
    Vector3TestClass::test_menor_que_float_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_float_vector_2() == get_float_vector_1(), expected_answer);
  }

  TEST_F(Vector3TestClass, igual_que_double_2) {
    Vector3TestClass::test_menor_que_double_2();
    bool const expected_answer = false;
    ASSERT_EQ(get_double_vector_2() == get_double_vector_1(), expected_answer);
  }

}  // namespace Vector3Test

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}