# Enable Google Test Library

add_executable(file_io
FileIO_test.cpp
)

add_executable(progargs progargs_test.cpp)

add_executable(vector3
    vector3_test.cpp)

add_executable(block
    block_test.cpp)

add_executable(grid
    grid_test.cpp)
# Library dependencies
target_link_libraries (file_io PRIVATE
sim
gtest
Microsoft.GSL::GSL)

# Library dependencies
target_link_libraries (progargs PRIVATE
sim
gtest
Microsoft.GSL::GSL)

# Library dependencies
target_link_libraries (vector3 PRIVATE
sim
gtest
Microsoft.GSL::GSL)

# Library dependencies
target_link_libraries (block PRIVATE
sim
gtest
Microsoft.GSL::GSL)

target_link_libraries (grid PRIVATE
sim
gtest
Microsoft.GSL::GSL)

