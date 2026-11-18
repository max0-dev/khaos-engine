#pragma once
#include <glm/glm.hpp>
#include <khaos/Input/Keys.h>
#include <khaos/Input/Mouse.h>

namespace KhaosCore {
    class InputManager {
      private:
        static InputManager& instance() {
            static InputManager sInstance;
            return sInstance;
        }

      public:
        static bool IsKeyDown(Key key);
        static glm::vec2 GetMousePosition();
        static CursorMode GetCursorMode();
        static void SetCursorMode(CursorMode mode);
    };
} // namespace KhaosCore
