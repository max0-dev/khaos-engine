#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"

using namespace KhaosCore;

GLVertexBufferObject::GLVertexBufferObject() : mWriteOffset(0), mBufferCapacity(0) {
    glGenBuffers(1, &mHandle);
}
GLVertexBufferObject::~GLVertexBufferObject() { glDeleteBuffers(1, &mHandle); }
void GLVertexBufferObject::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, mHandle); }
void GLVertexBufferObject::BufferData(GLsizeiptr size, const void* data, GLenum usage) {
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    mBufferCapacity = static_cast<size_t>(size);
}
void GLVertexBufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    mWriteOffset += static_cast<size_t>(size);
}
