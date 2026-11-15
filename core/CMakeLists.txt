# core: the library with the actual code. app and tests both link it.

file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(core STATIC ${CORE_SOURCES})
project_setup_target(core)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_compile_definitions(core PUBLIC $<$<CONFIG:Debug,RelWithDebInfo>:PROJECT_DEBUG>)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(core PUBLIC
    fmt::fmt
    glfw
    GLEW::GLEW
    OpenGL::GL
)
