# This file is located in the src directory

cmake_minimum_required(VERSION 3.28)

project(Compiler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
        main.cpp
        checks.cpp
        stages.cpp
        command.cpp
        log.cpp
)

add_executable(Driver ${SOURCES})
#target_link_libraries(Driver PRIVATE log)
# Set the module path for compilation
# target_include_directories(Driver PRIVATE ${CMAKE_SOURCE_DIR}/src)