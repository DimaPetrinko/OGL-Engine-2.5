#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::SetData(const void* data, const unsigned int size)
{
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	// glBufferData(GL_ARRAY_BUFFER, size of data we're pushing, the data itself, can be static, dynamic.. see documentation);
}

void VertexBuffer::Generate()
{
	GLCall(glGenBuffers(1, &rendererId));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
