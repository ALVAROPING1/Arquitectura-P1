# Enable Google Test Library

add_executable(utest
block_test.cpp
FileIO_test.cpp
progargs_test.cpp
grid_test.cpp
)
add_executable(vector3
    vector3_test.cpp)

# Library dependencies
target_link_libraries (utest PRIVATE
sim
gtest
Microsoft.GSL::GSL)

# Library dependencies
target_link_libraries (vector3 PRIVATE
sim
gtest
Microsoft.GSL::GSL)


