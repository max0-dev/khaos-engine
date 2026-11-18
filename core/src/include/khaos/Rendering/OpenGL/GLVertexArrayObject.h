#pragma once
#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"
#include <vector>

namespace KhaosCore {
    struct GLLayoutElement {
        unsigned int type;
        unsigned int count;
        bool normalized;
    };

    class GLVertexArrayLayout {
      private:
        std::vector<GLLayoutElement> mElements;
        unsigned int mStride{};

      public:
        GLVertexArrayLayout() = default;

        void PushFloat(unsigned int count);

        const std::vector<GLLayoutElement>& GetElements() const;
        unsigned int GetStride() const;

        static unsigned int GetSizeOfType(GLenum type) {
            switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
                break;
            }
            return 0;
        }
    };

    class GLVertexArrayObject {
      private:
        unsigned int mHandle;
        GLVertexArrayLayout mLayout;

      public:
        GLVertexArrayObject();
        GLVertexArrayObject(const GLVertexArrayObject&) = delete;
        GLVertexArrayObject(GLVertexArrayObject&&) = delete;
        GLVertexArrayObject& operator=(const GLVertexArrayObject&) = delete;
        GLVertexArrayObject& operator=(GLVertexArrayObject&&) = delete;
        ~GLVertexArrayObject();
        void Bind() const;
        void AddBuffer(const GLVertexBufferObject& vbo) const;
        GLVertexArrayLayout& GetLayout();
    };
} // namespace KhaosCore
