#pragma once
#include "khaos/App/Window.h"
#include <memory>

namespace KhaosCore {
    class App {
      protected:
        std::unique_ptr<Window> mWindow;
        virtual void Update() {}
        virtual void Render() {}

      public:
        App();
        App(const App&) = delete;
        App(App&&) = delete;
        App& operator=(const App&) = delete;
        App& operator=(App&&) = delete;
        void Run();
        virtual ~App() = default;
    };
} // namespace KhaosCore
