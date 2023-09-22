add_executable(fluid fluid.cpp)
target_include_directories(fluid PUBLIC ../common)
target_link_libraries(fluid PUBLIC sim PRIVATE Microsoft.GSL::GSL)
