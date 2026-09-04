add_library(${GPU_TARGET} STATIC
    gpu.cpp
)

target_include_directories(${GPU_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${GPU_TARGET}
    PRIVATE
        glad
    PUBLIC
        ${APPLICATION_TARGET}
)
