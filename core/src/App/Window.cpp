#include "khaos/App/Window.h"
#include "Logging/KCoreLog.h"
#include "khaos/tui/Tui.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <imgui.h>
#include <utility>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

KhaosCore::Window::Window() : KhaosCore::Window(WindowContext{}) {}

KhaosCore::Window::Window(WindowContext context) : mContext(std::move(context)) {
    int glfwSuccess = glfwInit();

    assert(glfwSuccess == GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    mPtr = glfwCreateWindow(
        mContext.width, mContext.height, mContext.title.c_str(), nullptr, nullptr
    );
    assert(mPtr != nullptr);

    glfwMakeContextCurrent(mPtr);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    assert(err == GLEW_OK);
    KLog::Info("Created a window!");

    glfwSetWindowSizeCallback(mPtr, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });

    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr; // the layout comes from code, no imgui.ini
    ImGui_ImplGlfw_InitForOpenGL(mPtr, true);
    ImGui_ImplOpenGL3_Init("#version 460"); // the engine's context is 4.6 core

    KhaosTui::Config config;
    config.FontPath =
        "/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-Regular.ttf"; // installed on your machine
    config.FontSize = 20.0F;
    config.Colors = KhaosTui::Theme::TokyoNight(); // or CatppuccinMocha(), GruvboxDark()
    // config.KittyThemeFile = "~/.config/kitty/...";   // optional: follows your kitty theme,
    // reloads live
    KhaosTui::Init(config);
}

KhaosCore::Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(mPtr);
    glfwTerminate();
}
void KhaosCore::Window::SwapBuffers() { glfwSwapBuffers(mPtr); }
bool KhaosCore::Window::ShouldClose() const { return glfwWindowShouldClose(mPtr); }
