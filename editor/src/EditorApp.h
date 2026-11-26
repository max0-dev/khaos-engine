#pragma once

struct GLFWwindow;

namespace khaos::editor {

class EditorApp {
public:
    EditorApp() = default;
    ~EditorApp();

    EditorApp(const EditorApp&) = delete;
    EditorApp& operator=(const EditorApp&) = delete;

    bool init(int width, int height, const char* title);
    void run();

private:
    void shutdown();
    void drawUi();

    GLFWwindow* m_window = nullptr;
    bool m_glfwInitialized = false;
    bool m_imguiInitialized = false;
    bool m_showDemo = false;
    float m_clearColor[4] = {0.08f, 0.08f, 0.10f, 1.0f};
};

} // namespace khaos::editor
