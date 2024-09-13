# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Set the project name
project(AllTests)

# Enable testing for this project
enable_testing()

# Locate the Google Test package
find_package(GTest REQUIRED)

# Ensure the log module is built as a library
add_library(log STATIC ../src/Log.cppm)
target_sources(log PRIVATE ../src/Log.cppm)
set_source_files_properties(log.cppm PROPERTIES LANGUAGE CXX)

# Add the test_logger executable
add_executable(test_logger TestLogger.cpp)
target_link_libraries(test_logger PRIVATE log GTest::GTest GTest::Main)

# Add the test_driver executable
add_executable(test_driver TestDriver.cpp ../src/Stages.cpp)
target_link_libraries(test_driver PRIVATE log GTest::GTest GTest::Main)

# Add all tests
add_executable(all_tests TestDriver.cpp TestLogger.cpp ../src/Stages.cpp ../src/Command.cpp)
target_link_libraries(all_tests PRIVATE log GTest::GTest GTest::Main)

# Add tests to the suite
add_test(NAME TestLogger COMMAND test_logger)
add_test(NAME TestDriver COMMAND test_driver)
add_test(NAME AllTests COMMAND all_tests)

# Add compile options for warnings
target_compile_options(all_tests PRIVATE -Wall -Wextra -Wpedantic)

# Ensure correct module flags are passed (for both Clang and GCC)

target_compile_options(log PRIVATE -fmodules)     # For Clang
target_compile_options(test_logger PRIVATE -fmodules)     # Ensure main.cpp can import the module