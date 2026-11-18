#include "khaos/Rendering/OpenGL/GLStaticMesh.h"
#include "Logging/KCoreLog.h"
#include <GL/glew.h>
#include <assimp/mesh.h>
#include <cstddef>
#include <format>
#include <vector>

using namespace KhaosCore;

namespace {
    // vec3 pos, vec3 normal, vec2 uv
    constexpr std::size_t FloatsPerVertex = 8;
    constexpr unsigned int UVChannel = 0;
} // namespace

GLStaticMesh::GLStaticMesh(const aiMesh* mesh) {
    if (mesh == nullptr) {
        KLog::Error("GLStaticMesh got a null mesh");
        return;
    }
    const char* name = mesh->mName.C_Str();

    // aiProcess_Triangulate leaves lines and points alone (aiProcess_SortByPType moves them
    // into their own meshes), so only triangle meshes are guaranteed 3 indices per face.
    // Triangulate also marks the meshes it split polygons in with the NGON flag.
    constexpr auto NGONFlag = static_cast<unsigned int>(aiPrimitiveType_NGONEncodingFlag);
    constexpr auto Triangles = static_cast<unsigned int>(aiPrimitiveType_TRIANGLE);
    if ((mesh->mPrimitiveTypes & ~NGONFlag) != Triangles) {
        KLog::Error(std::format("Mesh '{}' isn't made of triangles only, skipping it", name));
        return;
    }
    if (!mesh->HasFaces()) {
        KLog::Error(std::format("Mesh '{}' doesn't have faces", name));
        return;
    }
    if (!mesh->HasNormals()) {
        KLog::Error(std::format("Mesh '{}' doesn't have normals", name));
        return;
    }
    if (!mesh->HasTextureCoords(UVChannel)) {
        KLog::Error(std::format("Mesh '{}' doesn't have uv coords", name));
        return;
    }

    std::vector<float> vertices(static_cast<std::size_t>(mesh->mNumVertices) * FloatsPerVertex);
    for (std::size_t vertex = 0; vertex < mesh->mNumVertices; vertex++) {
        const auto& pos = mesh->mVertices[vertex];
        const auto& normal = mesh->mNormals[vertex];
        const auto& uv = mesh->mTextureCoords[UVChannel][vertex];
        float* out = &vertices[vertex * FloatsPerVertex];

        out[0] = pos.x;
        out[1] = pos.y;
        out[2] = pos.z;
        out[3] = normal.x;
        out[4] = normal.y;
        out[5] = normal.z;
        out[6] = uv.x;
        out[7] = uv.y;
    }

    std::vector<unsigned int> indices;
    indices.reserve(static_cast<std::size_t>(mesh->mNumFaces) * 3);
    for (std::size_t face = 0; face < mesh->mNumFaces; face++) {
        const aiFace& f = mesh->mFaces[face];
        if (f.mNumIndices != 3) {
            KLog::Error(std::format("Mesh '{}' has a face with {} indices", name, f.mNumIndices));
            return;
        }
        indices.insert(indices.end(), f.mIndices, f.mIndices + 3);
    }

    mVBO.BufferData(static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data());
    mEBO.BufferData(
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
        static_cast<unsigned int>(indices.size())
    );
}
