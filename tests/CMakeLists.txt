cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(YourProjectName VERSION 1.0 LANGUAGES CXX)

# Add the path to gmock headers
find_package(GTest REQUIRED)

# Source files for the project
set(SOURCES
        ../src/checks.cpp
        ../src/command.cpp
        ../src/stages.cpp
        ../src/log.cpp
)

# Test source files
set(TEST_SOURCES
        log_test.cpp
        stages_test.cpp
        checks_test.cpp
        command_test.cpp
)

# Enable testing
enable_testing()

# Loop over each test source file to create an executable and register it with CTest
foreach (TEST_SOURCE ${TEST_SOURCES})
    # Extract the test name from the source file name (without extension)
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Create an executable for each test
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCES})

    # Link the GTest libraries
    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest GTest::gtest_main)

    # Set properties for the executable
    set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

    # Register the test with CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach ()