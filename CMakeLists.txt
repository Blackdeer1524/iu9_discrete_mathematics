cmake_minimum_required(VERSION 3.22)
project(discrete)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -Wall -Werror -Wpedantic -Wextra -Wconversion)
