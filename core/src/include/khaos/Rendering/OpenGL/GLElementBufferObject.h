#pragma once
#include <GL/glew.h>

namespace KhaosCore {
    class GLElementBufferObject {
      private:
        unsigned int mHandle{};
        unsigned int mCount{};

      public:
        GLElementBufferObject() { glGenBuffers(1, &mHandle); }
        GLElementBufferObject(const GLElementBufferObject&) = delete;
        GLElementBufferObject(GLElementBufferObject&&) = delete;
        GLElementBufferObject& operator=(const GLElementBufferObject&) = delete;
        GLElementBufferObject& operator=(GLElementBufferObject&&) = delete;
        ~GLElementBufferObject() { glDeleteBuffers(1, &mHandle); }
        void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle); }
        [[nodiscard]] unsigned int GetCount() const { return mCount; }
        void BufferData(
            GLsizeiptr size, const void* data, unsigned int count, GLenum usage = GL_STATIC_DRAW
        ) {
            Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
            mCount = count;
        }
    };
} // namespace KhaosCore
