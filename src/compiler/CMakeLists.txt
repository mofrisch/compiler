
cmake_minimum_required(VERSION 3.28)
project(CompilerProject)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source file.
add_executable(CC compiler.cpp)