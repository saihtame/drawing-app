set(IO_TARGET drawing_app_io)

add_library(${IO_TARGET} STATIC
    io.cpp
)

target_link_libraries(${IO_TARGET}
    PUBLIC
        ${APPLICATION_TARGET}
)
