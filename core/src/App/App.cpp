#include "khaos/App/App.h"
#include "khaos/App/Window.h"
#include <GLFW/glfw3.h>
#include <fmt/base.h>
#include <memory>

using namespace KhaosCore;

App::App() { mWindow = std::make_unique<Window>(); }

void App::Run() {
    while (!mWindow->ShouldClose()) {
        glfwPollEvents();

        Update();

        Render();

        mWindow->SwapBuffers();
    }
}
