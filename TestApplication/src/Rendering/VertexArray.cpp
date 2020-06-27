#include "VertexArray.h"

namespace Rendering
{
	VertexArray::VertexArray(const bool& glInitialized)
	{
		if (!glInitialized) return;
		GLCall(glGenVertexArrays(1, &mRendererId));
	}

	VertexArray::~VertexArray()
	{
		if (mRendererId == 0) return;
		GLCall(glDeleteVertexArrays(1, &mRendererId));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(mRendererId));
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
			GLCall(glVertexAttribPointer(i, element.Count/*per object*/, element.Type, element.Normalized,
				layout.GetStride(), (const void*)offset));
			offset += element.Count/*total*/ * VertexBufferElement::GetSizeOfType(element.Type);
		}
		vb.Unbind();
		// glVertexAttribPointer(attribute index, elements count, GL_FLOAT, normalize (for 0 .. 255 byte or smth),
		// size of vertex in bytes (includes texture coords), starting index (in bytes));
		Unbind();

		//GLCall(glVertexAttribPointer(i, element.countPerAttribute, element.type, element.normalized,
		//		0?, (const void*)offset));
		//	offset += element.totalCount * VertexBufferElement::GetSizeOfType(element.type);
	}

	template<>
	void VertexBufferLayout::Push<float>(const unsigned int& count/*, countPerAttribute*/)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		// include in buffer element
		// stride += countPerAttribute * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(const unsigned int& count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned char>(const unsigned int& count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
}