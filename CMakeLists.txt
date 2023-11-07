cmake_minimum_required(VERSION 3.26)
project(fluid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=.*)

include(FetchContent)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

find_package(GTest CONFIG REQUIRED)

add_subdirectory(sim)
add_subdirectory(fluid)
add_subdirectory(debug)

enable_testing()
add_subdirectory(utest)
add_subdirectory(ftest)
