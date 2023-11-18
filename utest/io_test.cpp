#include "sim/io.hpp"
#include "sim/vector3.hpp"

#include <gtest/gtest.h>

namespace IOTest {
  constexpr double double_val{1.5};
  constexpr float float_val{1.5};
  constexpr Fluid::Vector3<float> vec_val{1.0, 0.5, 1.5};

  // pruebas de put
  // strings
  TEST(IOTest, put_string_1) {
    std::stringstream out{};
    Fluid::IO::put(out, "test");
    EXPECT_EQ(out.str(), "test");
  }

  TEST(IOTest, put_string_2) {
    std::stringstream out{};
    Fluid::IO::put(out, "test", "test2");
    EXPECT_EQ(out.str(), "testtest2");
  }

  // enteros
  TEST(IOTest, put_int) {
    std::stringstream out{};
    Fluid::IO::put(out, 1);
    EXPECT_EQ(out.str(), "1");
  }

  // doubles
  TEST(IOTest, put_double) {
    std::stringstream out{};
    Fluid::IO::put(out, double_val);
    EXPECT_EQ(out.str(), "1.5");
  }

  // mezclas
  TEST(IOTest, put_mix) {
    std::stringstream out{};
    Fluid::IO::put(out, "int: ", 2, " double: ", double_val);
    EXPECT_EQ(out.str(), "int: 2 double: 1.5");
  }

  // pruebas de put_line
  // strings
  TEST(IOTest, put_line_string_1) {
    std::stringstream out{};
    Fluid::IO::put_line(out, "test");
    EXPECT_EQ(out.str(), "test\n");
  }

  TEST(IOTest, put_line_string_2) {
    std::stringstream out{};
    Fluid::IO::put_line(out, "test", "test2");
    EXPECT_EQ(out.str(), "testtest2\n");
  }

  // enteros
  TEST(IOTest, put_line_int) {
    std::stringstream out{};
    Fluid::IO::put_line(out, 1);
    EXPECT_EQ(out.str(), "1\n");
  }

  // doubles
  TEST(IOTest, put_line_double) {
    std::stringstream out{};
    Fluid::IO::put_line(out, double_val);
    EXPECT_EQ(out.str(), "1.5\n");
  }

  // mezclas
  TEST(IOTest, put_line_mix) {
    std::stringstream out{};
    Fluid::IO::put_line(out, "int: ", 2, " double: ", double_val);
    EXPECT_EQ(out.str(), "int: 2 double: 1.5\n");
  }

  template <typename T>
  void write(std::ostream & out, T value) {
    // Permite escribir datos en binario
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    out.write(reinterpret_cast<char const *>(&value), sizeof(T));
  }

  template <typename T>
  bool read(std::istream & out, T & value) {
    // Permite leer datos en binario
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    return static_cast<bool>(out.read(reinterpret_cast<char *>(&value), sizeof(T)));
  }

  // pruebas de read
  // enteros
  TEST(IOTest, read_int_1) {
    std::stringstream buf{};
    write(buf, 2);
    int result{};
    ASSERT_EQ(Fluid::IO::read(buf, result), true);
    EXPECT_EQ(result, 2);
    EXPECT_EQ(Fluid::IO::read(buf, result), false);
  }

  TEST(IOTest, read_int_2) {
    std::stringstream buf{};
    write(buf, 2);
    write(buf, 4);
    int result1{};
    int result2{};
    ASSERT_EQ(Fluid::IO::read(buf, result1, result2), true);
    EXPECT_EQ(result1, 2);
    EXPECT_EQ(result2, 4);
    EXPECT_EQ(Fluid::IO::read(buf, result1), false);
  }

  // floats
  TEST(IOTest, read_float) {
    std::stringstream buf{};
    write(buf, float_val);
    float result{};
    ASSERT_EQ(Fluid::IO::read(buf, result), true);
    EXPECT_EQ(result, float_val);
    EXPECT_EQ(Fluid::IO::read(buf, result), false);
  }

  // vectores de floats
  TEST(IOTest, read_vector3) {
    std::stringstream buf{};
    write(buf, vec_val);
    Fluid::Vector3<float> result{};
    ASSERT_EQ(Fluid::IO::read(buf, result), true);
    EXPECT_EQ(result, vec_val);
    EXPECT_EQ(Fluid::IO::read(buf, result), false);
  }

  // mezclas
  TEST(IOTest, read_mix) {
    std::stringstream buf{};
    write(buf, float_val);
    write(buf, 3);
    write(buf, vec_val);
    float result_float{};
    int result_int{};
    Fluid::Vector3<float> result_vec{};
    ASSERT_EQ(Fluid::IO::read(buf, result_float, result_int, result_vec), true);
    EXPECT_EQ(result_float, float_val);
    EXPECT_EQ(result_int, 3);
    EXPECT_EQ(result_vec, vec_val);
    EXPECT_EQ(Fluid::IO::read(buf, result_int), false);
  }

  // Sobran datos de tamaño insuficiente para un valor pedido
  TEST(IOTest, read_leftover_data) {
    std::stringstream buf{};
    write(buf, 2);
    write(buf, 'A');
    int result{};
    ASSERT_EQ(Fluid::IO::read(buf, result), true);
    EXPECT_EQ(result, 2);
    EXPECT_EQ(Fluid::IO::read(buf, result), false);
  }

  // pruebas de write
  // enteros
  TEST(IOTest, write_int_1) {
    std::stringstream buf{};
    Fluid::IO::write(buf, 2);
    int result{};
    ASSERT_EQ(read(buf, result), true);
    EXPECT_EQ(result, 2);
    EXPECT_EQ(read(buf, result), false);
  }

  TEST(IOTest, write_int_2) {
    std::stringstream buf{};
    Fluid::IO::write(buf, 2, 4);
    int result{};
    ASSERT_EQ(read(buf, result), true);
    EXPECT_EQ(result, 2);
    ASSERT_EQ(read(buf, result), true);
    EXPECT_EQ(result, 4);
    EXPECT_EQ(read(buf, result), false);
  }

  // floats
  TEST(IOTest, write_float) {
    std::stringstream buf{};
    Fluid::IO::write(buf, float_val);
    float result{};
    ASSERT_EQ(read(buf, result), true);
    EXPECT_EQ(result, float_val);
    EXPECT_EQ(read(buf, result), false);
  }

  // vectores de floats
  TEST(IOTest, write_vector3) {
    std::stringstream buf{};
    Fluid::IO::write(buf, vec_val);
    Fluid::Vector3<float> result{};
    ASSERT_EQ(read(buf, result), true);
    EXPECT_EQ(result, vec_val);
    EXPECT_EQ(read(buf, result), false);
  }

  // mezclas
  TEST(IOTest, write_mix) {
    std::stringstream buf{};
    Fluid::IO::write(buf, float_val, 3, vec_val);
    float result_float{};
    int result_int{};
    Fluid::Vector3<float> result_vec{};
    ASSERT_EQ(read(buf, result_float), true);
    EXPECT_EQ(result_float, float_val);
    ASSERT_EQ(read(buf, result_int), true);
    EXPECT_EQ(result_int, 3);
    ASSERT_EQ(read(buf, result_vec), true);
    EXPECT_EQ(result_vec, vec_val);
    EXPECT_EQ(read(buf, result_int), false);
  }
}  // namespace IOTest
