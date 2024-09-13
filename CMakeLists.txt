cmake_minimum_required(VERSION 3.28)
project(Compiler VERSION 0.1.0 LANGUAGES C CXX)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # This ensures that only standard C++ is used, not GNU extensions
add_compile_options(-Wall -Wextra -Wpedantic)


# Set default build type if not provided
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Enable specific sanitizers for different testing cases
set(SANITIZER_FLAGS "")
if (SANITIZER_MODE STREQUAL "thread")
    set(SANITIZER_FLAGS "-fsanitize=thread")
elseif (SANITIZER_MODE STREQUAL "memory")
    set(SANITIZER_FLAGS "-fsanitize=memory")
else ()
    set(SANITIZER_FLAGS "-fsanitize=address,undefined")
endif ()

# Apply the sanitizer flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # No sanitizers in Debug mode
else ()
    # Enable sanitizers for other modes
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
endif ()

# Collect source files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

# Clang-Format Target
add_custom_target(
        clang-format
        COMMAND clang-format -i ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format"
)

# Google Test integration
find_package(GTest REQUIRED)
# Needed for pthread linking
find_package(Threads REQUIRED)
# google benchmark integration
find_package(benchmark REQUIRED)
# Boost integration
find_package(Boost REQUIRED COMPONENTS system filesystem)
# If Boost is found, include directories and link the libraries
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # TODO: Add the libraries to the target
else ()
    message(FATAL_ERROR "Boost not found")
endif ()


# RE/flex Lexer integration
option(ENABLE_LEXER "Enable RE/flex-based lexer" ON)

if (ENABLE_LEXER)
    find_program(REFLEX_EXECUTABLE reflex)
    if (NOT REFLEX_EXECUTABLE)
        message(FATAL_ERROR "RE/flex not found. Please install RE/flex.")
    endif ()

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
    if (NOT REFLEX_LIB)
        message(FATAL_ERROR "RE/flex library not found.")
    endif ()
endif ()

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Include subdirectory for driver
add_subdirectory(src)


# Compiler


# Lexer


add_subdirectory(tests)

