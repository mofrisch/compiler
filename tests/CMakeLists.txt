# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate the Google Test package
find_package(GTest REQUIRED)

add_executable(test_logger TestLogger.cpp)
target_link_libraries(test_logger PRIVATE log GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME MyTest COMMAND test_logger)