set(GPU_TARGET drawing_app_gpu)

add_library(${GPU_TARGET} STATIC
    gpu.cpp
)

target_link_libraries(${GPU_TARGET}
    PRIVATE
        glad
    PUBLIC
        ${APPLICATION_TARGET}
)
