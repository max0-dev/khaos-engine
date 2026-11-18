#include "khaos/App/App.h"
#include "khaos/Input/InputManager.h"

using namespace KhaosCore;

bool InputManager::IsKeyDown(Key key) {
    int status = glfwGetKey(App::instance().GetWindow()->GetNative(), Key_Utils::ToGLFW(key));
    if (status == GLFW_PRESS) {
        return true;
    } else {
        return false;
    }
}

glm::vec2 InputManager::GetMousePosition() {
    double xpos, ypos;
    glfwGetCursorPos(App::instance().GetWindow()->GetNative(), &xpos, &ypos);
    return glm::vec2((float)xpos, (float)ypos);
}

CursorMode InputManager::GetCursorMode() {
    if (glfwGetInputMode(App::instance().GetWindow()->GetNative(), GLFW_CURSOR) ==
        GLFW_CURSOR_NORMAL) {
        return CursorMode::Visible;
    } else if (glfwGetInputMode(App::instance().GetWindow()->GetNative(), GLFW_CURSOR) ==
               GLFW_CURSOR_DISABLED) {
        return CursorMode::Hidden;
    } else {
        return CursorMode::Unkown;
    }
}

void InputManager::SetCursorMode(CursorMode mode) {
    glfwSetInputMode(
        App::instance().GetWindow()->GetNative(), GLFW_CURSOR, CursorMode_Utils::ToGLFW(mode)
    );
}
