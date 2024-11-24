
# Add the source file.
add_executable(mcc ../log.cpp
        lexer.cpp)

# Custom command to copy the build result to c_sources
add_custom_command(TARGET mcc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mcc> ${CMAKE_SOURCE_DIR}/c_sources/
        COMMENT "Copying mcc to c_sources directory"
)
