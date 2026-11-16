#include "khaos/Rendering/OpenGL/GLStreamingTexture.h"
#include <GL/glew.h>

using namespace KhaosCore;

GLStreamingTexture::GLStreamingTexture(int width, int height) : mWidth(width), mHeight(height) {
    glCreateTextures(GL_TEXTURE_2D, 1, mTexture.AssignThroughPtr());
    glTextureStorage2D(mTexture.get(), 1, GL_RGBA8, mWidth, mHeight);

    glCreateFramebuffers(1, mReadFBO.AssignThroughPtr());
    glNamedFramebufferTexture(mReadFBO.get(), GL_COLOR_ATTACHMENT0, mTexture.get(), 0);
}
