#pragma once
#include "khaos/Rendering/OpenGL/GLStaticMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/cexport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>

namespace KhaosCore {
    class GLModel {
      private:
        std::vector<std::unique_ptr<GLStaticMesh>> mMeshes;
        void ProcessNode(const aiScene* scene, const aiNode* node);

      public:
        GLModel(const std::string& source);
        [[nodiscard]] const std::vector<std::unique_ptr<GLStaticMesh>>& GetMeshes() const {
            return mMeshes;
        }
    };
} // namespace KhaosCore
