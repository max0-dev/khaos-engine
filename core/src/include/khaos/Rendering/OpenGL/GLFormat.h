#pragma once
#include <GL/glew.h>
#include <array>

namespace KhaosCore {
    struct GLPixelFormat {
        GLenum internalFormat = GL_NONE;
        GLenum format = GL_NONE;
        std::array<GLint, 4> swizzle{GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};
        bool IsValid() const { return ((format != GL_NONE) && (internalFormat != GL_NONE)); }
    };

    GLPixelFormat GLPixelFormatFromChannels(int channels);
} // namespace KhaosCore
