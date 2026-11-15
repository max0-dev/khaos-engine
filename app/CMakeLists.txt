# app: the executable. Keep it thin; logic belongs in core where tests can reach it.

file(GLOB_RECURSE APP_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(app ${APP_SOURCES})
project_setup_target(app)

target_link_libraries(app PRIVATE
    core
)
