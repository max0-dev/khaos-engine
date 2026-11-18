# core: the library with the actual code. app and tests both link it.
file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(core STATIC ${CORE_SOURCES})
project_setup_target(core)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_include_directories(core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  ${Stb_INCLUDE_DIR}
)
target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(core PUBLIC $<$<CONFIG:Debug,RelWithDebInfo>:KHAOS_DEBUG>)

target_link_libraries(core PUBLIC
    glfw
    GLEW::GLEW
    OpenGL::GL
    spdlog::spdlog
    glm::glm
    assimp::assimp
    imgui::imgui
    khaos::tui
)
