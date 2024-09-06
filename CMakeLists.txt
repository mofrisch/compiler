cmake_minimum_required(VERSION 3.28)
project(Compiler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES src/compiler.cpp src/compiler_driver.cpp)

# Collect source files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

# Enable clang-tidy
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-llvmlibc-*,-modernize-use-trailing-return-type,-hicpp-named-parameter")

# Clang-Format Target (Optional: Use for formatting code)
add_custom_target(
     clang-format
     COMMAND clang-format -i ${ALL_SOURCE_FILES}
     COMMENT "Running clang-format"
)

# Enable testing
enable_testing()

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

# The compiler driver
add_executable(Driver src/compiler_driver.cpp )

# The compiler
add_executable(my_compiler src/compiler.cpp)
target_link_libraries(my_compiler PRIVATE /usr/local/lib64/libreflex_shared_lib.so)

# The lexer
add_executable(Lexer ${LEXER_OUTPUT})
target_link_libraries(Lexer PRIVATE reflex_shared_lib)




