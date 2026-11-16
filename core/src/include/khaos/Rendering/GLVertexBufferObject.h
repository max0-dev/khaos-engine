#pragma once
#include <GL/glew.h>
#include <cstddef>
namespace KhaosCore {
    class VertexBufferObject {
      private:
        unsigned int mHandle;
        size_t mWriteOffset;
        size_t mBufferCapacity;

      public:
        VertexBufferObject();
        ~VertexBufferObject();
        void Bind() const;
        void BufferData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
        [[nodiscard]] size_t GetCapacity() const { return mBufferCapacity; }
        [[nodiscard]] size_t GetWriteOffset() const { return mWriteOffset; }
    };
} // namespace KhaosCore
