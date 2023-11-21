# Enable Google Test Library

add_executable(utest
    main.cpp
    argumentparser_test.cpp
    block_test.cpp
    fileio_test.cpp
    grid_test.cpp
    io_test.cpp
    particle_test.cpp
    simulation_test.cpp
    vector3_test.cpp
)

target_link_libraries (utest PRIVATE
sim
gtest
Microsoft.GSL::GSL)
