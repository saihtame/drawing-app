set(DOMAIN_TARGET drawing_app_domain)
set(APPLICATION_TARGET drawing_app_application)
set(GPU_TARGET drawing_app_gpu)
set(IO_TARGET drawing_app_io)
set(PRESENTATION_TARGET drawing_app_presentation)

add_executable(${TARGET_NAME} main.cpp)

add_subdirectory(domain)
add_subdirectory(application)
add_subdirectory(gpu)
add_subdirectory(io)
add_subdirectory(presentation)

# Link libraries to executable
target_link_libraries(${TARGET_NAME} PRIVATE
    ${PRESENTATION_TARGET}
    ${IO_TARGET}
    ${GPU_TARGET}
)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${TARGET_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Copy shaders to bin
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:${TARGET_NAME}>/shaders
)

# Copy assets to bin
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
)
