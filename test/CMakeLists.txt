# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name to AllTests
project(AllTests)

# Enable testing for this project
enable_testing()

# Locate the Google Test package (assuming GTest is installed on your system)
find_package(GTest REQUIRED)

# Add the directory containing the main project source code
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add the test executable
add_executable(all_tests
        TestDriver.cpp   # Your test driver source
        TestLogger.cpp   # Your test logger source
)

# Link the test executable with the main project files and Google Test
target_link_libraries(all_tests
        PRIVATE
        GTest::GTest
        GTest::Main
)

# Add test files to the test executable
target_sources(all_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/driver/Stages.cpp  # Link with main source files
        ${CMAKE_SOURCE_DIR}/src/log/Logger.cpp  # Link the logger source
)

# Add the test executable to the test suite
add_test(NAME AllTests COMMAND all_tests)

# Optional: Add flags for better debugging or warnings during test compilation
target_compile_options(all_tests PRIVATE -Wall -Wextra -Wpedantic)
