cmake_minimum_required(VERSION 3.28)
project(Compiler VERSION 0.1.0 LANGUAGES C CXX)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)


# Set default build type if not provided
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Enable specific sanitizers for different testing cases
if(SANITIZER_MODE STREQUAL "thread")
    set(SANITIZER_FLAGS "-fsanitize=thread")
elseif(SANITIZER_MODE STREQUAL "memory")
    set(SANITIZER_FLAGS "-fsanitize=memory")
else()
    set(SANITIZER_FLAGS "-fsanitize=address,undefined")
endif()

# Apply the sanitizer flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${SANITIZER_FLAGS}")


# Collect source files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

# Clang-Format Target
add_custom_target(
    clang-format
    COMMAND clang-format -i ${ALL_SOURCE_FILES}
    COMMENT "Running clang-format"
)

# Google Test and Google Benchmark
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)  # For pthread linking
find_package(benchmark REQUIRED)

# RE/flex Lexer integration
find_program(REFLEX_EXECUTABLE reflex)
if(NOT REFLEX_EXECUTABLE)
    message(FATAL_ERROR "RE/flex not found. Please install RE/flex.")
endif()

set(LEXER_SOURCE "${CMAKE_SOURCE_DIR}/src/lexer.l")
set(LEXER_OUTPUT "${CMAKE_BINARY_DIR}/lexer.cpp")

add_custom_command(
    OUTPUT ${LEXER_OUTPUT}
    COMMAND ${REFLEX_EXECUTABLE} --flex ${LEXER_SOURCE} -o ${LEXER_OUTPUT}
    DEPENDS ${LEXER_SOURCE}
    COMMENT "Generating lexer.cpp with RE/flex"
)

# Find RE/flex shared library
find_library(REFLEX_LIB reflex_shared_lib HINTS /usr/local/lib64)
if(NOT REFLEX_LIB)
    message(FATAL_ERROR "RE/flex library not found.")
endif()

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Compiler driver
add_executable(Driver driver/compiler_driver.cpp driver/utilities.cpp)

# Copy the result file (Driver) to another directory after building
add_custom_command(TARGET Driver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:Driver>  # Source file (the result)
            ${CMAKE_SOURCE_DIR}/input/  # Destination directory
)

# Compiler
add_executable(my_compiler src/compiler.cpp)
target_link_libraries(my_compiler PRIVATE ${REFLEX_LIB})

# Lexer
add_executable(Lexer ${LEXER_OUTPUT})
target_link_libraries(Lexer PRIVATE ${REFLEX_LIB})

# Logging library
add_library(logging src/log.cpp)

# Test executable that uses GoogleTest
add_executable(log_test test/log_test.cpp)
target_link_libraries(log_test logging GTest::GTest GTest::Main)

# Enable testing
enable_testing()
add_test(NAME LogTest COMMAND log_test)
