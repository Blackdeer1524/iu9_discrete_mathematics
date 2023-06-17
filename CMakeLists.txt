cmake_minimum_required(VERSION 3.22)
project(discrete)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

find_package(GTest REQUIRED)
enable_testing()
include(GoogleTest)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -Wall -Werror -Wpedantic)
target_link_libraries(main GTest::gtest_main)
gtest_discover_tests(main)
