// funcion main customizada para tests

#include "FileIO_test.hpp"
#include <gtest/gtest.h>

namespace FileIOTest {
TEST(FileIOTestClass, foo) { ASSERT_EQ(true, true); }
} // namespace FileIOTest

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
