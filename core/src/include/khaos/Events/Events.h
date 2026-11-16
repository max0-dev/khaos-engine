#pragma once
#include "khaos/Events/Event.h"

namespace KEvent {
    class WindowResized : public KhaosCore::Event {
      private:
        int mWidth;
        int mHeight;

      public:
        WindowResized(int width, int height) : mWidth(width), mHeight(height) {}
        [[nodiscard]] int width() const { return mWidth; }
        [[nodiscard]] int height() const { return mHeight; }
    };
} // namespace KEvent
