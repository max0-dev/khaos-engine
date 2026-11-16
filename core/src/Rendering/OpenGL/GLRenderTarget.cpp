#include "khaos/Rendering/OpenGL/GLRenderTarget.h"
#include "Logging/KCoreLog.h"
#include "khaos/Rendering/SamplingMode.h"
#include <GL/glew.h>
#include <algorithm>
#include <format>

using namespace KhaosCore;

RenderTarget::RenderTarget(int width, int height, SamplingMode sampling)
    : mWidth(width), mHeight(height), mDesiredWidth(width), mDesiredHeight(height),
      mSampling(sampling) {
    if (mSampling == SamplingMode::Single) {
        glGenFramebuffers(1, mRenderFBO.AssignThroughPtr());
        Bind();

        glGenTextures(1, mRenderTexture.AssignThroughPtr());
        glBindTexture(GL_TEXTURE_2D, mRenderTexture.get());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture.get(), 0
        );

        glGenRenderbuffers(1, mRenderRBO.AssignThroughPtr());
        glBindRenderbuffer(GL_RENDERBUFFER, mRenderRBO.get());
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderRBO.get()
        );
    } else {
        int samples = GetSampleCount();
        glGenFramebuffers(1, mRenderFBO.AssignThroughPtr());
        Bind();

        glGenTextures(1, mRenderTexture.AssignThroughPtr());
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mRenderTexture.get());
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_TRUE
        );
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mRenderTexture.get(), 0
        );

        glGenRenderbuffers(1, mRenderRBO.AssignThroughPtr());
        glBindRenderbuffer(GL_RENDERBUFFER, mRenderRBO.get());
        glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height
        );
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderRBO.get()
        );

        glGenFramebuffers(1, mResolveFBO.AssignThroughPtr());
        glBindFramebuffer(GL_FRAMEBUFFER, mResolveFBO.get());

        glGenTextures(1, mResolveTexture.AssignThroughPtr());
        glBindTexture(GL_TEXTURE_2D, mResolveTexture.get());
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResolveTexture.get(), 0
        );
        CheckFBOStatus(mResolveFBO.get());
    }

    CheckFBOStatus(mRenderFBO.get());

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTarget::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mRenderFBO.get()); }

unsigned int RenderTarget::Resolve() {
    if (mSampling == SamplingMode::Single) {
        return mRenderTexture.get();
    } else {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mRenderFBO.get());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mResolveFBO.get());
        glBlitFramebuffer(
            0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST
        );
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return mResolveTexture.get();
    }
}

void RenderTarget::Resize(int newWidth, int newHeight) {
    mWidth = newWidth;
    mHeight = newHeight;

    if (mSampling == SamplingMode::Single) {
        glBindFramebuffer(GL_FRAMEBUFFER, mRenderFBO.get());
        glBindTexture(GL_TEXTURE_2D, mRenderTexture.get());
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture.get(), 0
        );

        glBindRenderbuffer(GL_RENDERBUFFER, mRenderRBO.get());
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderRBO.get()
        );

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        int samples = GetSampleCount();

        glBindFramebuffer(GL_FRAMEBUFFER, mRenderFBO.get());
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mRenderTexture.get());
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, mWidth, mHeight, GL_TRUE
        );
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mRenderTexture.get(), 0
        );

        glBindRenderbuffer(GL_RENDERBUFFER, mRenderRBO.get());
        glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, mWidth, mHeight
        );
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderRBO.get()
        );

        glBindFramebuffer(GL_FRAMEBUFFER, mResolveFBO.get());

        glBindTexture(GL_TEXTURE_2D, mResolveTexture.get());
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResolveTexture.get(), 0
        );
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTarget::SetDesiredSize(int newWidth, int newHeight) {
    if (newWidth > 0 && newHeight > 0) {
        mDesiredWidth = newWidth;
        mDesiredHeight = newHeight;
    }
}

bool RenderTarget::ApplyDesiredSize() {
    // using namespace KhaosEngine::Events;
    if (ShouldResize()) {
        Resize(mDesiredWidth, mDesiredHeight);
        CheckFBOStatus(mRenderFBO.get());
        if (mSampling != SamplingMode::Single)
            CheckFBOStatus(mResolveFBO.get());
        return true;
    }
    return false;
}

bool RenderTarget::ShouldResize() const {
    return mWidth != mDesiredWidth || mHeight != mDesiredHeight;
}

void RenderTarget::CheckFBOStatus(GLuint fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        KLog::Error(std::format("Framebuffer: {}, => is incomplete!", fbo));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int RenderTarget::GetSampleCount() const {
    int maxRBSamples = 0;
    int maxTexSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxRBSamples);
    glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &maxTexSamples);
    int samples = std::clamp(
        static_cast<int>(mSampling) * 4, 0, std::min(maxRBSamples, maxTexSamples)
    );
    return samples;
}
