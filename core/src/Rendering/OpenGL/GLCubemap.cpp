#include "khaos/Rendering/OpenGL/GLCubemap.h"
#include "Logging/KCoreLog.h"
#include "khaos/Rendering/OpenGL/GLFormat.h"
#include <format>
#include <memory>
#include <stb_image.h>
using namespace KhaosCore;

GLCubemap::GLCubemap(const std::array<std::string, 6>& sources) {
    glGenTextures(1, mHandle.AssignThroughPtr());
    Bind();

    int width, height, channels;

    stbi_set_flip_vertically_on_load(0);
    GLPixelFormat pixelFormat;
    int desiredChannels = 0;
    for (unsigned int i = 0; i < 6; i++) {
        std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data(
            stbi_load(sources[i].c_str(), &width, &height, &channels, desiredChannels),
            &stbi_image_free
        );
        if (data) {
            if (desiredChannels == 0) {
                pixelFormat = GLPixelFormatFromChannels(channels);
                if (!pixelFormat.IsValid()) {
                    KLog::Error(std::format("Invalid color format at {}", sources[i]));
                    continue;
                }
                desiredChannels = channels;
                glTexParameteriv(
                    GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, pixelFormat.swizzle.data()
                );
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                static_cast<GLint>(pixelFormat.internalFormat), width, height, 0,
                pixelFormat.format, GL_UNSIGNED_BYTE, data.get()
            );
        } else {
            KLog::Warn(std::format("Couldn't load cubemap texture at {}", sources[i]));
        }
    }
    stbi_set_flip_vertically_on_load(1);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void GLCubemap::Bind(GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle.get());
}
