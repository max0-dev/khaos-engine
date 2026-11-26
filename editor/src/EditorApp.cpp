#include "EditorApp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "khaos/core/Log.h"

namespace khaos::editor {

namespace {
void glfwErrorCallback(int code, const char* description) {
    KHAOS_ERROR("GLFW error {}: {}", code, description);
}
} // namespace

EditorApp::~EditorApp() { shutdown(); }

bool EditorApp::init(int width, int height, const char* title) {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        KHAOS_ERROR("Failed to initialize GLFW");
        return false;
    }
    m_glfwInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        KHAOS_ERROR("Failed to create window");
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (const GLenum err = glewInit(); err != GLEW_OK) {
        KHAOS_ERROR("Failed to initialize GLEW: {}", reinterpret_cast<const char*>(glewGetErrorString(err)));
        return false;
    }
    KHAOS_INFO("OpenGL {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    m_imguiInitialized = true;
    return true;
}

void EditorApp::run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUi();

        ImGui::Render();
        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}

void EditorApp::drawUi() {
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("ImGui Demo", nullptr, &m_showDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Khaos");
    ImGui::Text("Khaos Engine v%.*s", static_cast<int>(khaos::version().size()), khaos::version().data());
    ImGui::Text("%.1f FPS (%.3f ms)", io.Framerate, 1000.0f / io.Framerate);
    ImGui::ColorEdit3("Clear color", m_clearColor);
    ImGui::End();

    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }
}

void EditorApp::shutdown() {
    if (m_imguiInitialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_imguiInitialized = false;
    }
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    if (m_glfwInitialized) {
        glfwTerminate();
        m_glfwInitialized = false;
    }
}

} // namespace khaos::editor
