cmake_minimum_required(VERSION 3.26)
project(fluid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "−march=native")
set(CMAKE_CXX_CLANG_TIDY clang-tidy --checks=cppcoreguidelines-*)

include(FetchContent)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

add_subdirectory(sim)
add_subdirectory(fluid)

enable_testing()
add_subdirectory(utest)
add_subdirectory(ftest)
