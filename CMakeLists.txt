cmake_minimum_required(VERSION 3.28)
project(Compiler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect compiler and apply specific flags based on the compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using Clang compiler")
    add_compile_options(
        -fmodules-ts
        -fdeps-format=p1689r5
        -fmodule-mapper=${CMAKE_CURRENT_BINARY_DIR}/module-mapper.modmap
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC compiler")
    # No module flags for GCC, as it does not support them fully yet
    add_compile_options(-Wall)  # Add any specific GCC options here
endif()

# Set build types (Debug, Release, etc.)
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

# Option to set the build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


set(SOURCES src/compiler.cpp src/compiler_driver.cpp)

# Collect source files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

# Enable clang-tidy
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-llvmlibc-*,-modernize-use-trailing-return-type,-hicpp-named-parameter")

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

# Use find_library to locate RE/flex shared library
find_library(REFLEX_LIB reflex_shared_lib HINTS /usr/local/lib64)

if(NOT REFLEX_LIB)
    message(FATAL_ERROR "RE/flex library not found.")
endif()

# Add the include directory for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# The compiler driver
add_executable(Driver driver/compiler_driver.cpp driver/utilities.cpp)

# After building, copy the result file (Driver) to another directory
add_custom_command(TARGET Driver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:Driver>  # Source file (the result)
            ${CMAKE_SOURCE_DIR}/input/  # Destination directory
)


# The compiler
add_executable(my_compiler src/compiler.cpp)
target_link_libraries(my_compiler PRIVATE ${REFLEX_LIB})

# The lexer
add_executable(Lexer ${LEXER_OUTPUT})
target_link_libraries(Lexer PRIVATE ${REFLEX_LIB})

# Logginng library
add_library(logging src/log.cpp)


# Test executable that uses GoogleTest
add_executable(log_test test/log_test.cpp)
target_link_libraries(log_test logging GTest::GTest GTest::Main)

# Enable testing
enable_testing()
add_test(NAME LogTest COMMAND log_test)