#include "khaos/Rendering/OpenGL/GLVertexArrayObject.h"
#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"
#include <GL/glew.h>
#include <cstddef>
#include <vector>
using namespace KhaosCore;

VertexArrayLayout::VertexArrayLayout() : mStride(0) {}

void VertexArrayLayout::PushFloat(unsigned int count) {
    mElements.push_back({.type = GL_FLOAT, .count = count, .normalized = false});
    mStride += sizeof(float) * count;
}

const std::vector<LayoutElement>& VertexArrayLayout::GetElements() const { return mElements; }

unsigned int VertexArrayLayout::GetStride() const { return mStride; }

void VertexArrayObject::AddBuffer(const VertexBufferObject& vbo) const {
    Bind();
    vbo.Bind();

    const auto& elements = mLayout.GetElements();
    unsigned int offset = 0;
    for (size_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glVertexAttribPointer(
            i, element.count, element.type, element.normalized, mLayout.GetStride(), (void*)offset
        );
        glEnableVertexAttribArray(i);
        offset += VertexArrayLayout::GetSizeOfType(element.type) * element.count;
    }
}

void VertexArrayObject::Bind() const { glBindVertexArray(mHandle); }
VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &mHandle); }
VertexArrayObject::~VertexArrayObject() { glDeleteVertexArrays(1, &mHandle); }
VertexArrayLayout& VertexArrayObject::GetLayout() { return mLayout; }
