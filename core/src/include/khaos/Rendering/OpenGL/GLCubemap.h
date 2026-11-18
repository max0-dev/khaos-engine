#pragma once
#include "khaos/Rendering/OpenGL/GLHandle.h"
#include <array>
#include <string>

namespace KhaosCore {
    class GLCubemap {
      private:
        GLHandle<GLTextureDestructor> mHandle;

      public:
        GLCubemap(const std::array<std::string, 6>& sources);
        void Bind(GLenum unit = GL_TEXTURE0);
    };
} // namespace KhaosCore
