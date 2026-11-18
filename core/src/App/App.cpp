#include "khaos/App/App.h"
#include "khaos/App/Window.h"
#include <GLFW/glfw3.h>
#include <fmt/base.h>
#include <memory>

using namespace KhaosCore;

static App* sInstance = nullptr;

App::App() {
    mWindow = std::make_unique<Window>();
    sInstance = this;
    mRootNode = std::make_unique<Node>();
}

void App::Run() {
    while (!mWindow->ShouldClose()) {
        glfwPollEvents();

        Update();

        Render();

        mWindow->SwapBuffers();
    }
}

App& App::instance() { return *sInstance; }

App::~App() { sInstance = nullptr; }
