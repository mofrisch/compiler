cmake_minimum_required(VERSION 3.5.0)
project(Compiler VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -O2 -Wall -fomit-frame-pointer -fno-stack-check -fno-stack-protector")

# RE/flex Lexer integration
find_program(REFLEX_EXECUTABLE reflex)

if(NOT REFLEX_EXECUTABLE)
    message(FATAL_ERROR "RE/flex not found. Please install RE/flex.")
endif()
link_directories(/usr/local/lib64)

set(LEXER_SOURCE "${CMAKE_SOURCE_DIR}/src/lexer.l")
set(LEXER_OUTPUT "${CMAKE_BINARY_DIR}/lexer.cpp")

add_custom_command(
    OUTPUT ${LEXER_OUTPUT}
    COMMAND ${REFLEX_EXECUTABLE} --flex ${LEXER_SOURCE} -o ${LEXER_OUTPUT}
    DEPENDS ${LEXER_SOURCE}
    COMMENT "Generating lexer.cpp with RE/flex"
)

add_executable(Driver src/compiler_driver.cpp )

add_executable(my_compiler src/compiler.cpp)
target_link_libraries(my_compiler PRIVATE reflex_shared_lib)

add_executable(Lexer ${LEXER_OUTPUT})
target_link_libraries(Lexer PRIVATE reflex_shared_lib)


# CTest and Google Test
include(CTest)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)
add_executable(tests test/test.cpp)
target_link_libraries(tests GTest::GTest GTest::Main)
add_test(NAME tests COMMAND tests)

# Google benchmark
add_executable(bench bench/main.cpp ${SOURCES})
find_package(benchmark REQUIRED)
target_link_libraries(bench PRIVATE benchmark::benchmark)
