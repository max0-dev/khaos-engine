#include "khaos/Rendering/GLVertexBufferObject.h"

using namespace KhaosCore;

VertexBufferObject::VertexBufferObject() : mWriteOffset(0), mBufferCapacity(0) {
    glGenBuffers(1, &mHandle);
}
VertexBufferObject::~VertexBufferObject() { glDeleteBuffers(1, &mHandle); }
void VertexBufferObject::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, mHandle); }
void VertexBufferObject::BufferData(GLsizeiptr size, const void* data, GLenum usage) {
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    mBufferCapacity = size;
}
void VertexBufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    mWriteOffset += size;
}
