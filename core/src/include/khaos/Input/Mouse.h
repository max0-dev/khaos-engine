#pragma once

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>
namespace KhaosCore {
    enum class CursorMode {
        Unkown = -1,

        Visible,
        Hidden
    };

    namespace CursorMode_Utils {
        inline GLenum ToGLFW(CursorMode mode) {
            switch (mode) {
            case CursorMode::Visible:
                return GLFW_CURSOR_NORMAL;
            case CursorMode::Hidden:
                return GLFW_CURSOR_DISABLED;
            default:
                return NULL;
            }
        }
    } // namespace CursorMode_Utils

    enum class MouseButton : int32_t {
        Unknown = -1,

        Left = 0,
        Right = 1,
        Middle = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,

        _count
    };

    namespace MouseButton_Utils {
        inline int ToGLFW(MouseButton button) {
            switch (button) {
            case MouseButton::Left:
                return GLFW_MOUSE_BUTTON_LEFT;
            case MouseButton::Right:
                return GLFW_MOUSE_BUTTON_RIGHT;
            case MouseButton::Middle:
                return GLFW_MOUSE_BUTTON_MIDDLE;
            case MouseButton::Button4:
                return GLFW_MOUSE_BUTTON_4;
            case MouseButton::Button5:
                return GLFW_MOUSE_BUTTON_5;
            case MouseButton::Button6:
                return GLFW_MOUSE_BUTTON_6;
            case MouseButton::Button7:
                return GLFW_MOUSE_BUTTON_7;
            case MouseButton::Button8:
                return GLFW_MOUSE_BUTTON_8;
            default:
                return -1;
            }
        }

        inline MouseButton FromGLFW(int glfwButton) {
            switch (glfwButton) {
            case GLFW_MOUSE_BUTTON_LEFT:
                return MouseButton::Left;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return MouseButton::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_4:
                return MouseButton::Button4;
            case GLFW_MOUSE_BUTTON_5:
                return MouseButton::Button5;
            case GLFW_MOUSE_BUTTON_6:
                return MouseButton::Button6;
            case GLFW_MOUSE_BUTTON_7:
                return MouseButton::Button7;
            case GLFW_MOUSE_BUTTON_8:
                return MouseButton::Button8;
            default:
                return MouseButton::Unknown;
            }
        }

        inline std::string ToString(MouseButton button) {
            switch (button) {
            case MouseButton::Left:
                return "Left";
            case MouseButton::Right:
                return "Right";
            case MouseButton::Middle:
                return "Middle";
            case MouseButton::Button4:
                return "Button4";
            case MouseButton::Button5:
                return "Button5";
            case MouseButton::Button6:
                return "Button6";
            case MouseButton::Button7:
                return "Button7";
            case MouseButton::Button8:
                return "Button8";
            default:
                return "Unknown";
            }
        }

        inline MouseButton FromString(const std::string& name) {
            if (name == "Left")
                return MouseButton::Left;
            if (name == "Right")
                return MouseButton::Right;
            if (name == "Middle")
                return MouseButton::Middle;
            if (name == "Button4")
                return MouseButton::Button4;
            if (name == "Button5")
                return MouseButton::Button5;
            if (name == "Button6")
                return MouseButton::Button6;
            if (name == "Button7")
                return MouseButton::Button7;
            if (name == "Button8")
                return MouseButton::Button8;

            return MouseButton::Unknown;
        }
    } // namespace MouseButton_Utils
} // namespace KhaosCore
