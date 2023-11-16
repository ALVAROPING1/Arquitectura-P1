// Pruebas unitarias para progargs

#include <array>
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>
#include <span>

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
