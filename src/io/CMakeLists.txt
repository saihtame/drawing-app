add_library(${IO_TARGET} STATIC
    io.cpp
)

target_include_directories(${IO_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${IO_TARGET}
    PUBLIC
        ${APPLICATION_TARGET}
)
