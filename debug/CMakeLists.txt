add_executable(fluid-dbg main.cpp)
target_include_directories(fluid-dbg PUBLIC ../common)
target_link_libraries(fluid-dbg PUBLIC sim PRIVATE Microsoft.GSL::GSL)
set(CMAKE_CXX_CLANG_TIDY)
