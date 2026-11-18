#include "khaos/Rendering/OpenGL/GLModel.h"
#include "Logging/KCoreLog.h"
#include "khaos/Rendering/OpenGL/GLStaticMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <format>
#include <memory>
#include <string>

using namespace KhaosCore;

GLModel::GLModel(const std::string& source) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        source, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_SortByPType |
                    aiProcess_GenSmoothNormals | aiProcess_GenUVCoords
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        KLog::Error(std::format("Couldn't load model {}: {}", source, importer.GetErrorString()));
        return;
    }

    ProcessNode(scene, scene->mRootNode);
    if (mMeshes.empty()) {
        KLog::Warn(std::format("Model {} has no usable meshes", source));
    }
}

void GLModel::ProcessNode(const aiScene* scene, const aiNode* node) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto glMesh = std::make_unique<GLStaticMesh>(mesh);
        if (glMesh->IsValid()) {
            mMeshes.emplace_back(std::move(glMesh));
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(scene, node->mChildren[i]);
    }
}
