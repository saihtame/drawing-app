set(PRESENTATION_TARGET drawing_app_presentation)

add_library(${PRESENTATION_TARGET} STATIC
    app.cpp
)

target_link_libraries(${PRESENTATION_TARGET}
    PRIVATE
        SDL3::SDL3
        glad
        stb::image
    PUBLIC
        ${APPLICATION_TARGET}
)

