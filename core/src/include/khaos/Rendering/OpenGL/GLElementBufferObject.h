#pragma once
#include <GL/glew.h>

namespace KhaosCore {
    class ElementBufferObject {
      private:
        unsigned int mHandle{};
        unsigned int mCount{};

      public:
        ElementBufferObject() { glGenBuffers(1, &mHandle); }
        ElementBufferObject(const ElementBufferObject&) = delete;
        ElementBufferObject(ElementBufferObject&&) = delete;
        ElementBufferObject& operator=(const ElementBufferObject&) = delete;
        ElementBufferObject& operator=(ElementBufferObject&&) = delete;
        ~ElementBufferObject() { glDeleteBuffers(1, &mHandle); }
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
