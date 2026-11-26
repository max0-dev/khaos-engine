find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(khaos_editor
    src/main.cpp
    src/EditorApp.cpp
)
target_link_libraries(khaos_editor PRIVATE
    khaos::core
    khaos::options
    glfw
    GLEW::GLEW
    OpenGL::GL
    imgui::imgui
)
set_target_properties(khaos_editor PROPERTIES OUTPUT_NAME "KhaosEditor")
khaos_set_output_dirs(khaos_editor)
