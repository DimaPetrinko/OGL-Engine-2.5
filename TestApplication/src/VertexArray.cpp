#include "VertexArray.h"

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::Generate()
{
	GLCall(glGenVertexArrays(1, &rendererId));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(rendererId));
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
