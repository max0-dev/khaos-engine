#pragma once
#include "khaos/App/Window.h"
#include <memory>

namespace KhaosCore {
    class App {
      private:
        std::unique_ptr<Window> mWindow;

      public:
        App();
        void Run();
    };
} // namespace KhaosCore
