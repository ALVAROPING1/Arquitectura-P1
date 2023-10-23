add_executable(fluid-dbg main.cpp)
target_include_directories(fluid-dbg PUBLIC ../common)
target_link_libraries(fluid-dbg PUBLIC sim PRIVATE Microsoft.GSL::GSL)

add_executable(fluid-timed fluid-timed.cpp)
target_include_directories(fluid-timed PUBLIC ../common)
target_link_libraries(fluid-timed PUBLIC sim PRIVATE Microsoft.GSL::GSL)

add_executable(fdiff fdiff.cpp)
target_include_directories(fdiff PUBLIC ../common)
target_link_libraries(fdiff PUBLIC sim PRIVATE Microsoft.GSL::GSL)

set(CMAKE_CXX_CLANG_TIDY)
