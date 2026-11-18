#include "khaos/Rendering/OpenGL/GLTexture.h"
#include "Logging/KCoreLog.h"
#include "khaos/Rendering/OpenGL/GLFormat.h"
#include <format>
#include <memory>
#include <stb_image.h>
using namespace KhaosCore;

GLTexture::GLTexture(std::string source) {
    glGenTextures(1, mHandle.AssignThroughPtr());
    Bind();

    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image_data(
        stbi_load(source.c_str(), &width, &height, &channels, 0), &stbi_image_free
    );
    if (image_data) {
        GLPixelFormat pixelFormat = GLPixelFormatFromChannels(channels);
        if (!pixelFormat.IsValid()) {
            KLog::Error(std::format("Invalid color format at {}", source));
            return;
        }
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, pixelFormat.swizzle.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D, 0, static_cast<GLint>(pixelFormat.internalFormat), width, height, 0,
            pixelFormat.format, GL_UNSIGNED_BYTE, image_data.get()
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        KLog::Warn("Failed to load texture");
    }
}

void GLTexture::Bind(GLenum textureSlot) {
    glActiveTexture(textureSlot);
    glBindTexture(GL_TEXTURE_2D, mHandle.get());
}

GLuint GLTexture::GetHandle() const { return mHandle.get(); }
