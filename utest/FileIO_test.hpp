#pragma once

#include <fstream>
#include <gtest/gtest.h>

namespace FileIOTest {

class FileIOTestClass : public testing::Test {

    std::ifstream archivo1{};
    std::ofstream archivo2{};

    FileIOTestClass() = default;
    void SetUp() override{};
    void TearDown() override{};
};

} // namespace FileIOTest
