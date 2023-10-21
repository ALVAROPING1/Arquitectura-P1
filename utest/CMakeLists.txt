# Enable Google Test Library

add_executable(utest
block_test.cpp
FileIO_test.cpp
progargs_test.cpp
grid_test.cpp)

# Library dependencies
target_link_libraries (utest PRIVATE
sim
gtest
Microsoft.GSL::GSL)

