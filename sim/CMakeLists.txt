add_library(sim
   argumentparser.cpp
   fileio.cpp)
target_include_directories(sim PUBLIC ..)
target_link_libraries(sim PRIVATE Microsoft.GSL::GSL)
