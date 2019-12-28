#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	GLCall(glGenBuffers(1, &_rendererId));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &_rendererId));
}

void VertexBuffer::SetData(const void* data, const unsigned int size)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	// glBufferData(GL_ARRAY_BUFFER, size of data we're pushing, the data itself, can be static, dynamic.. see documentation);
	Unbind();
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
