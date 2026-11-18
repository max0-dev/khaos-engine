#pragma once

#include "khaos/Rendering/OpenGL/GLElementBufferObject.h"
#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"
#include <assimp/mesh.h>
namespace KhaosCore {
    class GLStaticMesh {
      private:
        GLVertexBufferObject mVBO;
        GLElementBufferObject mEBO;

      public:
        GLStaticMesh(const aiMesh* mesh);
        // False when the mesh was rejected (logged) and nothing was uploaded.
        [[nodiscard]] bool IsValid() const { return mEBO.GetCount() > 0; }
        const GLVertexBufferObject& GetVBO() const { return mVBO; }
        const GLElementBufferObject& GetEBO() const { return mEBO; }
    };
} // namespace KhaosCore
