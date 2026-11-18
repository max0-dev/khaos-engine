#include "khaos/Rendering/OpenGL/GLFormat.h"

using namespace KhaosCore;

GLPixelFormat KhaosCore::GLPixelFormatFromChannels(int channels) {
    GLPixelFormat pixelFormat;
    if (channels == 1) {
        pixelFormat.format = GL_RED;
        pixelFormat.internalFormat = GL_R8;
        pixelFormat.swizzle = {GL_RED, GL_RED, GL_RED, GL_ONE};
        // GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        // glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    } else if (channels == 2) {
        pixelFormat.format = GL_RG;
        pixelFormat.internalFormat = GL_RG8;
        pixelFormat.swizzle = {GL_RED, GL_RED, GL_RED, GL_GREEN};
        // GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
        // glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    } else if (channels == 3) {
        pixelFormat.format = GL_RGB;
        pixelFormat.internalFormat = GL_RGB8;
    } else if (channels == 4) {
        pixelFormat.format = GL_RGBA;
        pixelFormat.internalFormat = GL_RGBA8;
    }
    return pixelFormat;
}
