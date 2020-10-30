#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, unsigned int count) : count(count)
{
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
