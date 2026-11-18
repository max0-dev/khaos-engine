#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>
#include <vector>

namespace KhaosTests {
    // Hidden window with the same context the editor asks for (4.6 core).
    // Skips the test when there is no display or no GL 4.6 driver.
    class GLContext {
      private:
        GLFWwindow* mWindow = nullptr;

      public:
        GLContext();
        GLContext(const GLContext&) = delete;
        GLContext(GLContext&&) = delete;
        GLContext& operator=(const GLContext&) = delete;
        GLContext& operator=(GLContext&&) = delete;
        ~GLContext();
    };

    // Writes pixels (top row first) to a png in the temp folder and returns its path.
    std::string WritePng(
        const std::string& name, int width, int height, int channels,
        const std::vector<std::uint8_t>& pixels
    );
} // namespace KhaosTests
