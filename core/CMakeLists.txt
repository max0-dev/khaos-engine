# core: the library with the actual code. app and tests both link it.

file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(core STATIC ${CORE_SOURCES})
project_setup_target(core)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(core PUBLIC $<$<CONFIG:Debug,RelWithDebInfo>:KHAOS_DEBUG>)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(ftxui CONFIG REQUIRED)

target_link_libraries(core PUBLIC
    glfw
    GLEW::GLEW
    OpenGL::GL
    spdlog::spdlog
    ftxui::ftxui
)
