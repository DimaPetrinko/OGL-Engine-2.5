#include "Renderer.h"
#include "VertexBuffer.h"
#include "GL.h"

namespace Rendering
{
	VertexBuffer::VertexBuffer(const bool& glInitialized)
	{
		if (!glInitialized) return;
		GLCall(glGenBuffers(1, &_rendererId));
	}

	VertexBuffer::~VertexBuffer()
	{
		if (_rendererId == 0) return;
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
}
