#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::SetData(const unsigned int* data, const unsigned int count)
{
	this->count = count;
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
	// glBufferData(GL_ARRAY_BUFFER, size of data we're pushing, the data itself, can be static, dynamic.. see documentation);
}

void IndexBuffer::Generate()
{
	GLCall(glGenBuffers(1, &rendererId));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
