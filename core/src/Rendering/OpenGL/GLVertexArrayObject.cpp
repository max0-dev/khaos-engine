#include "khaos/Rendering/OpenGL/GLVertexArrayObject.h"
#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"
#include <GL/glew.h>
#include <cstddef>
#include <cstdint>
#include <vector>
using namespace KhaosCore;

void GLVertexArrayLayout::PushFloat(unsigned int count) {
    mElements.push_back({.type = GL_FLOAT, .count = count, .normalized = false});
    mStride += sizeof(float) * count;
}

const std::vector<GLLayoutElement>& GLVertexArrayLayout::GetElements() const { return mElements; }

unsigned int GLVertexArrayLayout::GetStride() const { return mStride; }

void GLVertexArrayObject::AddBuffer(const GLVertexBufferObject& vbo) const {
    Bind();
    vbo.Bind();

    const auto& elements = mLayout.GetElements();
    std::uintptr_t offset = 0;
    for (size_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glVertexAttribPointer(
            i, element.count, element.type, element.normalized, mLayout.GetStride(),
            reinterpret_cast<const void*>(offset)
        );
        glEnableVertexAttribArray(i);
        offset += GLVertexArrayLayout::GetSizeOfType(element.type) * element.count;
    }
}

void GLVertexArrayObject::Bind() const { glBindVertexArray(mHandle); }
GLVertexArrayObject::GLVertexArrayObject() { glGenVertexArrays(1, &mHandle); }
GLVertexArrayObject::~GLVertexArrayObject() { glDeleteVertexArrays(1, &mHandle); }
GLVertexArrayLayout& GLVertexArrayObject::GetLayout() { return mLayout; }
