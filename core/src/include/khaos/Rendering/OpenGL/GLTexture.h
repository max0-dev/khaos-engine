#pragma once
#include "khaos/Rendering/OpenGL/GLHandle.h"
#include <string>

namespace KhaosCore {
    class GLTexture {
      private:
        GLHandle<GLTextureDestructor> mHandle;

      public:
        GLTexture(std::string source);
        void Bind(GLenum textureSlot = GL_TEXTURE0);
        GLuint GetHandle() const;
    };
} // namespace KhaosCore
