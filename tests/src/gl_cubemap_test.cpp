#include "GLTestUtils.h"
#include "khaos/Rendering/OpenGL/GLCubemap.h"
#include "khaos/Rendering/OpenGL/GLTexture.h"
#include <GL/glew.h>
#include <array>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

using namespace KhaosCore;
using namespace KhaosTests;

namespace {
    // GLCubemap has no GetHandle(), so bind it and ask GL what is bound.
    GLuint HandleOf(GLCubemap& cubemap) {
        cubemap.Bind();
        GLint bound = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &bound);
        return static_cast<GLuint>(bound);
    }

    GLint FaceParam(GLuint cubemap, int face, GLenum param) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        GLint value = 0;
        glGetTexLevelParameteriv(
            static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), 0, param, &value
        );
        return value;
    }

    GLint TexParam(GLuint cubemap, GLenum param) {
        GLint value = 0;
        glGetTextureParameteriv(cubemap, param, &value);
        return value;
    }

    // Reads one face through its face target. glGetTextureSubImage would refuse
    // (GL_INVALID_OPERATION) on a cubemap with a missing face, this does not.
    std::vector<std::uint8_t> ReadFace(
        GLuint cubemap, int face, GLenum format, int channels, int size
    ) {
        std::vector<std::uint8_t> out(static_cast<std::size_t>(size * size * channels));
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(
            static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), 0, format,
            GL_UNSIGNED_BYTE, out.data()
        );
        return out;
    }

    // 2x2 RGBA face whose pixels all differ and encode the face index,
    // so a swapped face or a flipped row shows up.
    std::vector<std::uint8_t> FacePixels(int face) {
        const auto f = static_cast<std::uint8_t>(face * 40);
        return {
            f, 1, 0, 255, f, 2, 0, 255, // top row
            f, 3, 0, 255, f, 4, 0, 255, // bottom row
        };
    }
    // Six 2x2 RGBA faces from FacePixels, written as <name>0.png ... <name>5.png.
    std::array<std::string, 6> FaceFiles(const std::string& name) {
        std::array<std::string, 6> sources;
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const int face = static_cast<int>(i);
            sources.at(i) = WritePng(name + std::to_string(face), 2, 2, 4, FacePixels(face));
        }
        return sources;
    }
} // namespace

TEST_CASE("GLCubemap uploads six faces in order and without flipping", "[gl][cubemap]") {
    const GLContext context;
    std::array<std::string, 6> sources = FaceFiles("cube_face");
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    REQUIRE(handle != 0);
    for (int face = 0; face < 6; ++face) {
        INFO("face " << face);
        CHECK(FaceParam(handle, face, GL_TEXTURE_WIDTH) == 2);
        CHECK(FaceParam(handle, face, GL_TEXTURE_HEIGHT) == 2);
        CHECK(FaceParam(handle, face, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGBA8);
        // Cubemap faces keep the image's top row first (no flip).
        CHECK(ReadFace(handle, face, GL_RGBA, 4, 2) == FacePixels(face));
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap sets linear filtering and clamps on all three axes", "[gl][cubemap]") {
    const GLContext context;
    std::array<std::string, 6> sources = FaceFiles("cube_params");
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    CHECK(TexParam(handle, GL_TEXTURE_MIN_FILTER) == GL_LINEAR);
    CHECK(TexParam(handle, GL_TEXTURE_MAG_FILTER) == GL_LINEAR);
    CHECK(TexParam(handle, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
    CHECK(TexParam(handle, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
    CHECK(TexParam(handle, GL_TEXTURE_WRAP_R) == GL_CLAMP_TO_EDGE);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap converts every face to the first face's channel count", "[gl][cubemap]") {
    const GLContext context;
    // First face RGB (3x3, rows not 4-byte aligned), the rest gray or RGBA.
    std::vector<std::uint8_t> rgb;
    for (int i = 0; i < 9; ++i) {
        rgb.insert(
            rgb.end(),
            {static_cast<std::uint8_t>(i), static_cast<std::uint8_t>(i + 100),
             static_cast<std::uint8_t>(i + 200)}
        );
    }
    const std::vector<std::uint8_t> gray = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::vector<std::uint8_t> rgba;
    for (int i = 0; i < 9; ++i) {
        rgba.insert(rgba.end(), {7, 8, 9, 128});
    }
    std::array<std::string, 6> sources = {
        WritePng("cube_mixed0", 3, 3, 3, rgb),  WritePng("cube_mixed1", 3, 3, 1, gray),
        WritePng("cube_mixed2", 3, 3, 4, rgba), WritePng("cube_mixed3", 3, 3, 3, rgb),
        WritePng("cube_mixed4", 3, 3, 3, rgb),  WritePng("cube_mixed5", 3, 3, 3, rgb),
    };
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    for (int face = 0; face < 6; ++face) {
        INFO("face " << face);
        CHECK(FaceParam(handle, face, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGB8);
    }
    CHECK(ReadFace(handle, 0, GL_RGB, 3, 3) == rgb);

    std::vector<std::uint8_t> grayAsRgb;
    for (const std::uint8_t g : gray) {
        grayAsRgb.insert(grayAsRgb.end(), {g, g, g});
    }
    CHECK(ReadFace(handle, 1, GL_RGB, 3, 3) == grayAsRgb);

    std::vector<std::uint8_t> rgbaAsRgb;
    for (int i = 0; i < 9; ++i) {
        rgbaAsRgb.insert(rgbaAsRgb.end(), {7, 8, 9}); // alpha dropped
    }
    CHECK(ReadFace(handle, 2, GL_RGB, 3, 3) == rgbaAsRgb);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap stores grayscale faces as R8 swizzled to gray", "[gl][cubemap]") {
    const GLContext context;
    const std::vector<std::uint8_t> gray = {10, 20, 30, 40};
    std::array<std::string, 6> sources;
    for (std::size_t i = 0; i < sources.size(); ++i) {
        sources.at(i) = WritePng("cube_gray" + std::to_string(i), 2, 2, 1, gray);
    }
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    CHECK(FaceParam(handle, 0, GL_TEXTURE_INTERNAL_FORMAT) == GL_R8);
    std::array<GLint, 4> swizzle{};
    glGetTextureParameteriv(handle, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());
    CHECK(swizzle == std::array<GLint, 4>{GL_RED, GL_RED, GL_RED, GL_ONE});
    CHECK(ReadFace(handle, 3, GL_RED, 1, 2) == gray);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap with a missing face still loads the other five", "[gl][cubemap]") {
    const GLContext context;
    std::array<std::string, 6> sources = FaceFiles("cube_missing");
    sources.at(2) = "does/not/exist.png";
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    REQUIRE(handle != 0);
    CHECK(FaceParam(handle, 2, GL_TEXTURE_WIDTH) == 0);
    for (const int face : {0, 1, 3, 4, 5}) {
        INFO("face " << face);
        CHECK(FaceParam(handle, face, GL_TEXTURE_WIDTH) == 2);
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap with a missing first face takes the format from the next one", "[gl][cubemap]") {
    const GLContext context;
    std::array<std::string, 6> sources = FaceFiles("cube_nofirst");
    sources.at(0) = "does/not/exist.png";
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    for (int face = 1; face < 6; ++face) {
        INFO("face " << face);
        CHECK(FaceParam(handle, face, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGBA8);
        CHECK(ReadFace(handle, face, GL_RGBA, 4, 2) == FacePixels(face));
    }
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLCubemap::Bind binds to the requested texture unit", "[gl][cubemap]") {
    const GLContext context;
    const std::array<std::string, 6> sources = FaceFiles("cube_bind");
    GLCubemap cubemap(sources);
    const GLuint handle = HandleOf(cubemap);

    cubemap.Bind(GL_TEXTURE5);
    GLint active = 0;
    GLint bound = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &bound);
    CHECK(active == GL_TEXTURE5);
    CHECK(static_cast<GLuint>(bound) == handle);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture still flips after a GLCubemap turned flipping off", "[gl][cubemap][texture]") {
    const GLContext context;
    std::array<std::string, 6> sources = FaceFiles("cube_then_tex");
    const GLCubemap cubemap(sources);

    const GLTexture texture(WritePng("tex_after_cube", 2, 2, 4, FacePixels(0)));
    std::vector<std::uint8_t> out(16);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTextureImage(texture.GetHandle(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 16, out.data());
    const std::vector<std::uint8_t> flipped = {
        0, 3, 0, 255, 0, 4, 0, 255, // file's bottom row first
        0, 1, 0, 255, 0, 2, 0, 255,
    };
    CHECK(out == flipped);
}
