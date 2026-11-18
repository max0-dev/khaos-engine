#include "GLTestUtils.h"
#include "khaos/Rendering/OpenGL/GLTexture.h"
#include <GL/glew.h>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <vector>

using namespace KhaosCore;
using namespace KhaosTests;

namespace {
    GLint LevelParam(GLuint texture, GLint level, GLenum param) {
        GLint value = 0;
        glGetTextureLevelParameteriv(texture, level, param, &value);
        return value;
    }

    std::vector<std::uint8_t> ReadBack(GLuint texture, GLenum format, int channels, int w, int h) {
        std::vector<std::uint8_t> out(static_cast<std::size_t>(w * h * channels));
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTextureImage(
            texture, 0, format, GL_UNSIGNED_BYTE, static_cast<GLsizei>(out.size()), out.data()
        );
        return out;
    }
} // namespace

TEST_CASE("GLTexture uploads an RGBA png flipped to GL's bottom-up rows", "[gl][texture]") {
    const GLContext context;
    // File rows go top to bottom: red green / blue white.
    const std::vector<std::uint8_t> file = {
        255, 0,   0,   255, 0,   255, 0,   255, // top
        0,   0,   255, 255, 255, 255, 255, 128, // bottom
    };
    const GLTexture texture(WritePng("rgba", 2, 2, 4, file));
    const GLuint handle = texture.GetHandle();

    REQUIRE(handle != 0);
    REQUIRE(glIsTexture(handle) == GL_TRUE);
    CHECK(LevelParam(handle, 0, GL_TEXTURE_WIDTH) == 2);
    CHECK(LevelParam(handle, 0, GL_TEXTURE_HEIGHT) == 2);
    CHECK(LevelParam(handle, 0, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGBA8);
    CHECK(LevelParam(handle, 1, GL_TEXTURE_WIDTH) == 1); // mipmaps were generated

    // GL row 0 is the bottom of the image, so the file's bottom row comes first.
    const std::vector<std::uint8_t> expected = {
        0,   0,   255, 255, 255, 255, 255, 128, // bottom
        255, 0,   0,   255, 0,   255, 0,   255, // top
    };
    CHECK(ReadBack(handle, GL_RGBA, 4, 2, 2) == expected);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture uploads RGB rows that are not 4-byte aligned", "[gl][texture]") {
    const GLContext context;
    // 3x3 RGB: 9 bytes per row, which breaks with the default unpack alignment of 4.
    std::vector<std::uint8_t> file;
    for (int i = 0; i < 9; ++i) {
        file.insert(
            file.end(), {static_cast<std::uint8_t>(i * 10), static_cast<std::uint8_t>(i * 20),
                         static_cast<std::uint8_t>(i * 25)}
        );
    }
    const GLTexture texture(WritePng("rgb", 3, 3, 3, file));
    const GLuint handle = texture.GetHandle();

    CHECK(LevelParam(handle, 0, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGB8);
    std::vector<std::uint8_t> expected;
    for (std::ptrdiff_t row = 2; row >= 0; --row) { // flipped
        expected.insert(
            expected.end(), file.begin() + (row * 9), file.begin() + ((row + 1) * 9)
        );
    }
    CHECK(ReadBack(handle, GL_RGB, 3, 3, 3) == expected);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture stores grayscale as R8 and swizzles it to gray", "[gl][texture]") {
    const GLContext context;
    const std::vector<std::uint8_t> file = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    const GLTexture texture(WritePng("gray", 3, 3, 1, file));
    const GLuint handle = texture.GetHandle();

    CHECK(LevelParam(handle, 0, GL_TEXTURE_INTERNAL_FORMAT) == GL_R8);
    std::array<GLint, 4> swizzle{};
    glGetTextureParameteriv(handle, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());
    CHECK(swizzle == std::array<GLint, 4>{GL_RED, GL_RED, GL_RED, GL_ONE});
    const std::vector<std::uint8_t> expected = {70, 80, 90, 40, 50, 60, 10, 20, 30};
    CHECK(ReadBack(handle, GL_RED, 1, 3, 3) == expected);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture stores gray+alpha as RG8 and swizzles it to gray with alpha", "[gl][texture]") {
    const GLContext context;
    const std::vector<std::uint8_t> file = {100, 200, 50, 25}; // 2x1: (gray, alpha) pairs
    const GLTexture texture(WritePng("grayalpha", 2, 1, 2, file));
    const GLuint handle = texture.GetHandle();

    CHECK(LevelParam(handle, 0, GL_TEXTURE_INTERNAL_FORMAT) == GL_RG8);
    std::array<GLint, 4> swizzle{};
    glGetTextureParameteriv(handle, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());
    CHECK(swizzle == std::array<GLint, 4>{GL_RED, GL_RED, GL_RED, GL_GREEN});
    CHECK(ReadBack(handle, GL_RG, 2, 2, 1) == file);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture with a missing file still gives a valid, empty texture", "[gl][texture]") {
    const GLContext context;
    const GLTexture texture("does/not/exist.png");
    const GLuint handle = texture.GetHandle();

    REQUIRE(handle != 0);
    CHECK(glIsTexture(handle) == GL_TRUE);
    CHECK(LevelParam(handle, 0, GL_TEXTURE_WIDTH) == 0);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("GLTexture::Bind binds to the requested texture unit", "[gl][texture]") {
    const GLContext context;
    const std::vector<std::uint8_t> file = {255, 255, 255, 255};
    GLTexture texture(WritePng("bind", 1, 1, 4, file));

    texture.Bind(GL_TEXTURE3);
    GLint active = 0;
    GLint bound = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
    CHECK(active == GL_TEXTURE3);
    CHECK(static_cast<GLuint>(bound) == texture.GetHandle());
    CHECK(glGetError() == GL_NO_ERROR);
}
