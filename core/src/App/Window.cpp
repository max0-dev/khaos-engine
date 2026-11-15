#include "khaos/App/Window.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <utility>

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
}

KhaosCore::Window::~Window() {
    glfwDestroyWindow(mPtr);
    glfwTerminate();
}
void KhaosCore::Window::SwapBuffers() { glfwSwapBuffers(mPtr); }
bool KhaosCore::Window::ShouldClose() const { return glfwWindowShouldClose(mPtr); }
