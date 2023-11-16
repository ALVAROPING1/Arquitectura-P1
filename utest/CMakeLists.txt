# Enable Google Test Library

add_executable(main
    main.cpp
    argumentparser_test.cpp
    block_test.cpp
    fileio_test.cpp
    grid_test.cpp
    io_test.cpp
    vector3_test.cpp
)

target_link_libraries (main PRIVATE
sim
gtest
Microsoft.GSL::GSL)
