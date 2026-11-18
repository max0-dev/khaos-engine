#include "GLTestUtils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace KhaosTests;

GLContext::GLContext() {
    if (glfwInit() != GLFW_TRUE) {
        SKIP("glfwInit failed (no display?)");
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    mWindow = glfwCreateWindow(16, 16, "gl test", nullptr, nullptr);
    if (mWindow == nullptr) {
        glfwTerminate();
        SKIP("could not create a GL 4.6 context");
    }
    glfwMakeContextCurrent(mWindow);
    glewExperimental = GL_TRUE;
    REQUIRE(glewInit() == GLEW_OK);
    while (glGetError() != GL_NO_ERROR) {} // glew can leave an error behind
}

GLContext::~GLContext() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

std::string KhaosTests::WritePng(
    const std::string& name, int width, int height, int channels,
    const std::vector<std::uint8_t>& pixels
) {
    const auto path = std::filesystem::temp_directory_path() / ("khaos_" + name + ".png");
    REQUIRE(
        stbi_write_png(path.c_str(), width, height, channels, pixels.data(), width * channels)
        != 0
    );
    return path.string();
}
