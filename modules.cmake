# file: ./modules.cmake

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LIBCXX_PATH "/home/mo/Development/llvm-project/build-libcxx")

file(COPY "${LIBCXX_PATH}/modules/c++/v1/"
     DESTINATION "${PROJECT_BINARY_DIR}/c++std/")

function(add_module target)
  add_library(${target} STATIC)
  target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${ARGN})
endfunction()

add_module(std "${PROJECT_BINARY_DIR}/c++std/std.cppm")

target_include_directories(std PUBLIC "${LIBCXX_PATH}/include/c++/v1")
target_compile_options(std PRIVATE
    -Wno-reserved-module-identifier
    -Wno-reserved-user-defined-literal
)
target_compile_options(std PUBLIC -nostdlib++ -nostdinc++)
target_link_options(std PUBLIC -nostdlib++)
target_link_libraries(std PRIVATE
  "${LIBCXX_PATH}/lib/libc++.a"
  "${LIBCXX_PATH}/lib/libc++abi.a"
  "${LIBCXX_PATH}/lib/libunwind.a"
)
set_target_properties(std PROPERTIES OUTPUT_NAME "c++std")
