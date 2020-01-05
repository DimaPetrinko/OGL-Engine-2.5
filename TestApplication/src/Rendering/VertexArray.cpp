#include "Rendering.h"
#include "VertexArray.h"

namespace Rendering
{
	VertexArray::VertexArray(const bool& glInitialized)
	{
		if (!glInitialized) return;
		GLCall(glGenVertexArrays(1, &_rendererId));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &_rendererId));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(_rendererId));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		unsigned int offset = 0;
		const auto& elements = layout.GetElements();
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
				layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		// glVertexAttribPointer(attribute index, elements count, GL_FLOAT, normalize (for 0 .. 255 byte or smth),
		// size of vertex in bytes (includes texture coords), starting index (in bytes));
		Unbind();
	}

	template<>
	void VertexBufferLayout::Push<float>(const unsigned int& count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(const unsigned int& count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned char>(const unsigned int& count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
}