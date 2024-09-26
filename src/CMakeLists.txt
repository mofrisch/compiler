# This file is located in the src directory

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
        main.cpp
        checks.cpp
        stages.cpp
        command.cpp
        log.cpp
        lexer.cpp
        lexer.cpp
)

add_executable(mcc ${SOURCES})

target_link_libraries(mcc ${Boost_LIBRARIES})