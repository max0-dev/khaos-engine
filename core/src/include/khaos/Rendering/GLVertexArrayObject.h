#pragma once
#include "khaos/Rendering/GLVertexBufferObject.h"
#include <vector>

namespace KhaosCore {
    struct LayoutElement {
        unsigned int type;
        unsigned int count;
        bool normalized;
    };

    class VertexArrayLayout {
      private:
        std::vector<LayoutElement> mElements;
        unsigned int mStride;

      public:
        VertexArrayLayout();

        void PushFloat(unsigned int count);

        [[nodiscard]] const std::vector<LayoutElement>& GetElements() const;
        [[nodiscard]] unsigned int GetStride() const;

        static unsigned int GetSizeOfType(GLenum type) {
            switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
                break;
            }
            return 0;
        }
    };

    class VertexArrayObject {
      private:
        unsigned int mHandle;
        VertexArrayLayout mLayout;

      public:
        VertexArrayObject();
        ~VertexArrayObject();
        void Bind() const;
        void AddBuffer(const VertexBufferObject& vbo) const;
        VertexArrayLayout& GetLayout();
    };
} // namespace KhaosCore
