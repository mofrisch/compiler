cmake_minimum_required(VERSION 3.28)

# Project name
project(Compiler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the module library
add_library(log STATIC Log.cpp)


# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)
# add_subdirectory(log)

# Add the source files
set(SOURCES
        main.cpp
        Checks.cpp
        Stages.cpp
        Command.cpp
        Log.cpp
)

# Add the executable
add_executable(Driver ${SOURCES})

# Link the log library
target_link_libraries(Driver PRIVATE log)
# Set the module path for compilation
target_include_directories(Driver PRIVATE ${CMAKE_SOURCE_DIR}/src)