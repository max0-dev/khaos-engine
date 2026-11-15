#include "khaos/App/App.h"
#include "khaos/App/Window.h"
#include <GLFW/glfw3.h>
#include <memory>

KhaosCore::App::App() { mWindow = std::make_unique<KhaosCore::Window>(); }

void KhaosCore::App::Run() {
    while (!mWindow->ShouldClose()) {
        glfwPollEvents();

        mWindow->SwapBuffers();
    }
}
