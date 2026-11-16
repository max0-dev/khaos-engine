# app: the executable. Keep it thin; logic belongs in core where tests can reach it.

file(GLOB_RECURSE EDITOR_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(editor ${EDITOR_SOURCES})
project_setup_target(editor)

target_include_directories(editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_include_directories(editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(editor PRIVATE
    core
)
