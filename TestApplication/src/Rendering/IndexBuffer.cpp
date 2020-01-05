#include "Rendering.h"
#include "IndexBuffer.h"

namespace Rendering
{
	IndexBuffer::IndexBuffer(const bool& glInitialized)
	{
		if (!glInitialized) return;
		GLCall(glGenBuffers(1, &_rendererId));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &_rendererId));
	}

	void IndexBuffer::SetData(const unsigned int* data, const unsigned int count)
	{
		Bind();
		this->count = count;
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
		// glBufferData(GL_ARRAY_BUFFER, size of data we're pushing, the data itself, can be static, dynamic.. see documentation);
		Unbind();
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
