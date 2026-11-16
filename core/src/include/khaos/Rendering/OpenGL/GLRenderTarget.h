#pragma once
#include "khaos/Rendering/OpenGL/GLHandle.h"
#include "khaos/Rendering/SamplingMode.h"
namespace KhaosCore {
    class RenderTarget {
      private:
        SamplingMode mSampling;
        int mWidth;
        int mHeight;
        int mDesiredWidth;
        int mDesiredHeight;
        GLHandle<GLFrameBufferDestructor> mRenderFBO;
        GLHandle<GLTextureDestructor> mRenderTexture;
        GLHandle<GLRenderBufferDestructor> mRenderRBO;

        GLHandle<GLFrameBufferDestructor> mResolveFBO;
        GLHandle<GLTextureDestructor> mResolveTexture;
        static void CheckFBOStatus(GLuint fbo);
        [[nodiscard]] int GetSampleCount() const;
        void Resize(int newWidth, int newHeight);

      public:
        RenderTarget(int width, int height, SamplingMode sampling);
        void Bind();
        unsigned int Resolve();
        void SetDesiredSize(int newWidth, int newHeight);
        bool ApplyDesiredSize();
        [[nodiscard]] bool ShouldResize() const;
        [[nodiscard]] int GetWidth() const { return mWidth; }
        [[nodiscard]] int GetHeight() const { return mHeight; }
    };
} // namespace KhaosCore
