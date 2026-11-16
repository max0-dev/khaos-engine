#pragma once
#include "GL/glew.h"
#include <type_traits>
namespace KhaosCore {
    class IHandleDestructor {
      public:
        static void Delete(GLuint handle);
        virtual ~IHandleDestructor() = default;
    };
    struct GLShaderDestructor : public IHandleDestructor {
        static void Delete(GLuint handle) { glDeleteShader(handle); }
    };
    struct GLProgramDestructor : public IHandleDestructor {
        static void Delete(GLuint handle) { glDeleteProgram(handle); }
    };
    struct GLFrameBufferDestructor : public IHandleDestructor {
        static void Delete(GLuint handle) { glDeleteFramebuffers(1, &handle); }
    };
    struct GLTextureDestructor : public IHandleDestructor {
        static void Delete(GLuint handle) { glDeleteTextures(1, &handle); }
    };
    struct GLRenderBufferDestructor : public IHandleDestructor {
        static void Delete(GLuint handle) { glDeleteRenderbuffers(1, &handle); }
    };

    template <class T, std::enable_if_t<std::is_base_of_v<IHandleDestructor, T>, int> = 0>
    class GLHandle {
      private:
        GLuint mHandle = 0;

      public:
        GLHandle() = default;
        GLHandle(GLHandle&&) = delete;
        GLHandle& operator=(GLHandle&&) = delete;
        GLHandle& operator=(const GLHandle&) = delete;
        GLHandle(const GLHandle&) = delete;
        explicit GLHandle(GLuint handle) : mHandle(handle) {}
        void Reset() {
            if (mHandle != 0) {
                T::Delete(mHandle);
                mHandle = 0;
            }
        }
        void Assign(GLuint handle) {
            if (mHandle != handle) {
                Reset();
                mHandle = handle;
            }
        }
        GLuint* AssignThroughPtr() {
            Reset();
            return &mHandle;
        }
        ~GLHandle() { Reset(); }
        [[nodiscard]] GLuint get() const { return mHandle; }
    };
} // namespace KhaosCore
