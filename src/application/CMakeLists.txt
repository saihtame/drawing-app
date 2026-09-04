set(APPLICATION_TARGET drawing_app_application)

add_library(${APPLICATION_TARGET} STATIC
    application.cpp
)

target_link_libraries(${APPLICATION_TARGET}
    PUBLIC
        ${DOMAIN_TARGET}
)
