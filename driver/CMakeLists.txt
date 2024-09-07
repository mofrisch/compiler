# driver/CMakeLists.txt
# Add the driver executable and related source files
add_executable(Driver main.cpp Checks.cpp Stages.cpp)

# Link any necessary libraries (if needed, e.g., logging or other libraries)
target_link_libraries(Driver PRIVATE logging)

# Post-build: Copy the result file (Driver) to another directory after building
add_custom_command(TARGET Driver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:Driver>  # Source file (the result)
            ${CMAKE_SOURCE_DIR}/input/  # Destination directory
)

# If you need specific include directories or compiler options for the driver, add them here:
target_include_directories(Driver PRIVATE ${CMAKE_SOURCE_DIR}/include)
