cmake_minimum_required(VERSION 3.5.0)
project(Compiler VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -O2 -Wall -fomit-frame-pointer -fno-stack-check -fno-stack-protector")

add_executable(Compiler main.cpp)

# CTest and Google Test
include(CTest)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)
add_executable(tests test/test.cpp)
target_link_libraries(tests GTest::GTest GTest::Main)


# Google benchmark
add_executable(bench bench/main.cpp ${SOURCES})
find_package(benchmark REQUIRED)
target_link_libraries(bench PRIVATE benchmark::benchmark)