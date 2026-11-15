#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace KhaosCore {
    struct WindowContext {
        std::string title = "Window";
        int width = 800;
        int height = 800;
    };

    class Window {
      private:
        GLFWwindow* mPtr;
        WindowContext mContext;

      public:
        explicit Window(WindowContext context);
        Window();
        Window(const Window&) = default;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = default;
        Window& operator=(Window&&) = delete;
        ~Window();

        [[nodiscard]] bool ShouldClose() const;
        void SwapBuffers();
    };
}; // namespace KhaosCore
