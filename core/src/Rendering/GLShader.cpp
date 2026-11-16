#include "khaos/Rendering/GLShader.h"
#include "Logging/KCoreLog.h"
#include "khaos/Rendering/GLHandle.h"
#include <format>
#include <fstream>
#include <sstream>
#include <string>

using namespace KhaosCore;

GLShader::GLShader(std::string vertexPath, std::string fragmentPath) {
    std::string sVertex;
    std::string sFragment;

    KLog::Info(std::format("At file: {}", vertexPath));
    if (std::ifstream file{vertexPath}) {
        std::stringstream ss;
        ss << file.rdbuf();
        sVertex = ss.str();
    } else {
        KLog::Warn("Couldn't open file");
        return;
    }

    KLog::Info(std::format("At file: {}", fragmentPath));
    if (std::ifstream file{fragmentPath}) {
        std::stringstream ss;
        ss << file.rdbuf();
        sFragment = ss.str();
    } else {
        KLog::Warn("Couldn't open file");
        return;
    }

    GLHandle<GLShaderDestructor> vertex(glCreateShader(GL_VERTEX_SHADER));
    GLHandle<GLShaderDestructor> fragment(glCreateShader(GL_FRAGMENT_SHADER));

    const char* sVertex_c = sVertex.c_str();
    const char* sFragment_c = sFragment.c_str();

    glShaderSource(vertex.get(), 1, &sVertex_c, nullptr);
    glShaderSource(fragment.get(), 1, &sFragment_c, nullptr);

    glCompileShader(vertex.get());
    glCompileShader(fragment.get());

    int vSuccess;
    glGetShaderiv(vertex.get(), GL_COMPILE_STATUS, &vSuccess);

    if (!vSuccess) {
        int len;
        glGetShaderiv(vertex.get(), GL_INFO_LOG_LENGTH, &len);
        std::string info_log;
        info_log.resize(len);
        glGetShaderInfoLog(vertex.get(), len, &len, &info_log[0]);
        KLog::Warn("Vertex shader compilation error: " + info_log);
        return;
    }

    int fSuccess;
    glGetShaderiv(fragment.get(), GL_COMPILE_STATUS, &fSuccess);

    if (!fSuccess) {
        int len;
        glGetShaderiv(fragment.get(), GL_INFO_LOG_LENGTH, &len);
        std::string info_log;
        info_log.resize(len);
        glGetShaderInfoLog(fragment.get(), len, &len, &info_log[0]);
        KLog::Warn("Fragment shader compilation error: " + info_log);
        return;
    }

    mHandle.Assign(glCreateProgram());

    glAttachShader(mHandle.get(), vertex.get());
    glAttachShader(mHandle.get(), fragment.get());

    glLinkProgram(mHandle.get());

    int pSuccess;
    glGetProgramiv(mHandle.get(), GL_LINK_STATUS, &pSuccess);
    if (!pSuccess) {
        int len;
        glGetProgramiv(mHandle.get(), GL_INFO_LOG_LENGTH, &len);
        std::string info_log;
        info_log.resize(len);
        glGetProgramInfoLog(mHandle.get(), len, &len, &info_log[0]);
        KLog::Warn("Shader program validation error: " + info_log);
        mHandle.Reset();
        return;
    }

    // Cleanup

    glDetachShader(mHandle.get(), vertex.get());

    glDetachShader(mHandle.get(), fragment.get());
}

void GLShader::Bind() const { glUseProgram(mHandle.get()); }
