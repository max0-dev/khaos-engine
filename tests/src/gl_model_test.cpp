#include "GLTestUtils.h"
#include "khaos/Rendering/OpenGL/GLModel.h"
#include "khaos/Rendering/OpenGL/GLStaticMesh.h"
#include <GL/glew.h>
#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using namespace KhaosCore;
using namespace KhaosTests;

namespace {
    struct TestVertex {
        aiVector3D pos;
        aiVector3D normal;
        aiVector3D uv;
    };

    // Builds an aiMesh in memory. aiMesh/aiFace free their arrays with delete[], so
    // everything is allocated with new[] and owned by the returned mesh.
    std::unique_ptr<aiMesh> MakeMesh(
        const std::vector<TestVertex>& vertices,
        const std::vector<std::vector<unsigned int>>& faces,
        unsigned int primitiveTypes = aiPrimitiveType_TRIANGLE, bool withNormals = true,
        bool withUVs = true
    ) {
        auto mesh = std::make_unique<aiMesh>();
        mesh->mPrimitiveTypes = primitiveTypes;
        mesh->mNumVertices = static_cast<unsigned int>(vertices.size());
        mesh->mVertices = new aiVector3D[vertices.size()];
        if (withNormals) {
            mesh->mNormals = new aiVector3D[vertices.size()];
        }
        if (withUVs) {
            mesh->mTextureCoords[0] = new aiVector3D[vertices.size()];
            mesh->mNumUVComponents[0] = 2;
        }
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            mesh->mVertices[i] = vertices[i].pos;
            if (withNormals) {
                mesh->mNormals[i] = vertices[i].normal;
            }
            if (withUVs) {
                mesh->mTextureCoords[0][i] = vertices[i].uv;
            }
        }

        mesh->mNumFaces = static_cast<unsigned int>(faces.size());
        mesh->mFaces = new aiFace[faces.size()];
        for (std::size_t f = 0; f < faces.size(); ++f) {
            mesh->mFaces[f].mNumIndices = static_cast<unsigned int>(faces[f].size());
            mesh->mFaces[f].mIndices = new unsigned int[faces[f].size()];
            for (std::size_t i = 0; i < faces[f].size(); ++i) {
                mesh->mFaces[f].mIndices[i] = faces[f][i];
            }
        }
        return mesh;
    }

    // A unit quad in the XY plane facing +Z, as two triangles.
    std::unique_ptr<aiMesh> MakeQuad() {
        return MakeMesh(
            {
                {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}},
                {{1, 0, 0}, {0, 0, 1}, {1, 0, 0}},
                {{1, 1, 0}, {0, 0, 1}, {1, 1, 0}},
                {{0, 1, 0}, {0, 0, 1}, {0, 1, 0}},
            },
            {{0, 1, 2}, {0, 2, 3}}
        );
    }

    // The buffer objects don't expose their handles, so bind them and ask GL which one is bound.
    template <typename T> std::vector<T> ReadBuffer(GLenum bindingQuery) {
        GLint handle = 0;
        glGetIntegerv(bindingQuery, &handle);
        GLint64 size = 0;
        glGetNamedBufferParameteri64v(static_cast<GLuint>(handle), GL_BUFFER_SIZE, &size);
        std::vector<T> out(static_cast<std::size_t>(size) / sizeof(T));
        glGetNamedBufferSubData(static_cast<GLuint>(handle), 0, size, out.data());
        return out;
    }

    std::vector<float> ReadVertices(const GLStaticMesh& mesh) {
        mesh.GetVBO().Bind();
        return ReadBuffer<float>(GL_ARRAY_BUFFER_BINDING);
    }

    // The element buffer binding is VAO state, so read it back through a scratch VAO.
    std::vector<unsigned int> ReadIndices(const GLStaticMesh& mesh) {
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        mesh.GetEBO().Bind();
        auto indices = ReadBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER_BINDING);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);
        return indices;
    }

    std::string WriteObj(const std::string& name, const std::string& contents) {
        const auto path = std::filesystem::temp_directory_path() / ("khaos_" + name + ".obj");
        std::ofstream(path) << contents;
        return path.string();
    }
} // namespace

// ---------------------------------------------------------------------------------------------
// GLStaticMesh

TEST_CASE("GLStaticMesh uploads interleaved position, normal, uv and the indices", "[gl][mesh]") {
    const GLContext context;
    const auto quad = MakeQuad();
    const GLStaticMesh mesh(quad.get());

    const std::vector<float> expectedVertices = {
        0, 0, 0, 0, 0, 1, 0, 0, // vertex 0: pos, normal, uv
        1, 0, 0, 0, 0, 1, 1, 0, //
        1, 1, 0, 0, 0, 1, 1, 1, //
        0, 1, 0, 0, 0, 1, 0, 1, //
    };
    CHECK(ReadVertices(mesh) == expectedVertices);
    CHECK(mesh.GetVBO().GetCapacity() == expectedVertices.size() * sizeof(float));

    CHECK(mesh.GetEBO().GetCount() == 6);
    CHECK(ReadIndices(mesh) == std::vector<unsigned int>{0, 1, 2, 0, 2, 3});
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLStaticMesh refuses meshes without normals or uvs", "[gl][mesh]") {
    const GLContext context;
    const std::vector<TestVertex> triangle = {
        {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}},
        {{1, 0, 0}, {0, 0, 1}, {1, 0, 0}},
        {{0, 1, 0}, {0, 0, 1}, {0, 1, 0}},
    };

    SECTION("no normals") {
        const auto source = MakeMesh(
            triangle, {{0, 1, 2}}, aiPrimitiveType_TRIANGLE, /*withNormals=*/false
        );
        const GLStaticMesh mesh(source.get());
        CHECK(mesh.GetEBO().GetCount() == 0);
        CHECK(mesh.GetVBO().GetCapacity() == 0);
    }
    SECTION("no uvs") {
        const auto source = MakeMesh(
            triangle, {{0, 1, 2}}, aiPrimitiveType_TRIANGLE, /*withNormals=*/true,
            /*withUVs=*/false
        );
        const GLStaticMesh mesh(source.get());
        CHECK(mesh.GetEBO().GetCount() == 0);
        CHECK(mesh.GetVBO().GetCapacity() == 0);
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

// Finding: the uv channel loop keeps the *last* channel that has coordinates, so a mesh with a
// lightmap uv set in channel 1 is drawn with the lightmap uvs instead of the material ones.
TEST_CASE("GLStaticMesh takes uvs from the first channel", "[gl][mesh]") {
    const GLContext context;
    const auto quad = MakeQuad();
    quad->mTextureCoords[1] = new aiVector3D[quad->mNumVertices];
    quad->mNumUVComponents[1] = 2;
    for (unsigned int i = 0; i < quad->mNumVertices; ++i) {
        quad->mTextureCoords[1][i] = {0.5F, 0.5F, 0}; // second set, must not be used
    }
    const GLStaticMesh mesh(quad.get());

    const auto vertices = ReadVertices(mesh);
    REQUIRE(vertices.size() == 4 * 8);
    for (unsigned int i = 0; i < 4; ++i) {
        CHECK(vertices[(i * 8) + 6] == quad->mTextureCoords[0][i].x);
        CHECK(vertices[(i * 8) + 7] == quad->mTextureCoords[0][i].y);
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

// Finding: the index loop always reads 3 indices per face, but aiProcess_Triangulate leaves line
// and point primitives alone. A line face has 2 indices, so this reads past mIndices
// (heap-buffer-overflow under ASan).
TEST_CASE("GLStaticMesh skips line meshes instead of reading 3 indices per face", "[gl][mesh]") {
    const GLContext context;
    const auto lines = MakeMesh(
        {
            {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}},
            {{1, 0, 0}, {0, 0, 1}, {1, 0, 0}},
            {{1, 1, 0}, {0, 0, 1}, {1, 1, 0}},
        },
        {{0, 1}, {1, 2}}, aiPrimitiveType_LINE
    );
    const GLStaticMesh mesh(lines.get());

    CHECK(mesh.GetEBO().GetCount() == 0);
    CHECK(glGetError() == GL_NO_ERROR);
}

// ---------------------------------------------------------------------------------------------
// GLModel

namespace {
    // Two objects, each one triangle with positions, uvs and normals.
    constexpr const char* TwoTrianglesObj = R"(
v 0 0 0
v 1 0 0
v 0 1 0
v 2 0 0
v 3 0 0
v 2 1 0
vt 0 0
vt 1 0
vt 0 1
vn 0 0 1
o first
f 1/1/1 2/2/1 3/3/1
o second
f 4/1/1 5/2/1 6/3/1
)";
} // namespace

// Finding: ProcessNode is never called, so mMeshes stays empty for every file.
TEST_CASE("GLModel loads every mesh in the file", "[gl][model]") {
    const GLContext context;
    const GLModel model(WriteObj("two_triangles", TwoTrianglesObj));

    REQUIRE(model.GetMeshes().size() == 2);
    for (const auto& mesh : model.GetMeshes()) {
        CHECK(mesh->GetEBO().GetCount() == 3);
        CHECK(mesh->GetVBO().GetCapacity() == sizeof(float) * 3 * 8);
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

// Finding: the import flags don't ask assimp for normals, and GLStaticMesh refuses meshes
// without them, so any OBJ exported without normals loads as nothing.
TEST_CASE("GLModel generates normals when the file has none", "[gl][model]") {
    const GLContext context;
    const GLModel model(WriteObj("no_normals", R"(
v 0 0 0
v 1 0 0
v 0 1 0
vt 0 0
vt 1 0
vt 0 1
f 1/1 2/2 3/3
)"));

    REQUIRE(model.GetMeshes().size() == 1);
    CHECK(model.GetMeshes()[0]->GetEBO().GetCount() == 3);
    CHECK(glGetError() == GL_NO_ERROR);
}

// Finding: same 3-indices-per-face assumption as above, reached through a real file. Lines must
// be split off (aiProcess_SortByPType) and skipped, leaving only the triangle mesh.
TEST_CASE("GLModel keeps the triangles of a file that also has lines", "[gl][model]") {
    const GLContext context;
    const GLModel model(WriteObj("triangle_and_line", R"(
v 0 0 0
v 1 0 0
v 0 1 0
vt 0 0
vt 1 0
vt 0 1
vn 0 0 1
f 1/1/1 2/2/1 3/3/1
l 1 2
)"));

    REQUIRE(model.GetMeshes().size() == 1);
    CHECK(model.GetMeshes()[0]->GetEBO().GetCount() == 3);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLModel with a missing file has no meshes", "[gl][model]") {
    const GLContext context;
    const GLModel model("does/not/exist.obj");

    CHECK(model.GetMeshes().empty());
    CHECK(glGetError() == GL_NO_ERROR);
}
