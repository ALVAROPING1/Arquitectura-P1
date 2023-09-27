add_library(sim block.cpp grid.cpp particle.cpp progargs.cpp fileIO.cpp)
target_include_directories(sim PUBLIC ..)
target_link_libraries(sim PRIVATE Microsoft.GSL::GSL)
