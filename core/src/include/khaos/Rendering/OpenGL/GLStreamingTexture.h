#pragma once
#include "khaos/Rendering/OpenGL/GLHandle.h"

namespace KhaosCore {
    class GLStreamingTexture {
      private:
        GLHandle<GLTextureDestructor> mTexture;
        GLHandle<GLFrameBufferDestructor> mReadFBO;
        int mWidth;
        int mHeight;

      public:
        GLStreamingTexture(int width, int height);
    };
} // namespace KhaosCore
