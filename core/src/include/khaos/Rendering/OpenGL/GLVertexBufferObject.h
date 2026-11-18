#pragma once
#include <GL/glew.h>
#include <cstddef>
namespace KhaosCore {
    class GLVertexBufferObject {
      private:
        unsigned int mHandle;
        size_t mWriteOffset;
        size_t mBufferCapacity;

      public:
        GLVertexBufferObject();
        GLVertexBufferObject(const GLVertexBufferObject&) = delete;
        GLVertexBufferObject(GLVertexBufferObject&&) = delete;
        GLVertexBufferObject& operator=(const GLVertexBufferObject&) = delete;
        GLVertexBufferObject& operator=(GLVertexBufferObject&&) = delete;
        ~GLVertexBufferObject();
        void Bind() const;
        void BufferData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
        size_t GetCapacity() const { return mBufferCapacity; }
        size_t GetWriteOffset() const { return mWriteOffset; }
    };
} // namespace KhaosCore
