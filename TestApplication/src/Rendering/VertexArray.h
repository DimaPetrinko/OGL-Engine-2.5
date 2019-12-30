#pragma once

#include <vector>
#include "Rendering.h"
#include "GraphicsObject.h"
#include "VertexBuffer.h"

namespace Rendering
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
				case GL_FLOAT: return 4;
				case GL_UNSIGNED_INT: return 4;
				case GL_UNSIGNED_BYTE: return 1;
				default: return 0;
			}
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride{};
	public:
		template <typename T>
		void Push(const unsigned int& count);

		inline const std::vector<VertexBufferElement>& GetElements() const { return elements; }
		inline unsigned int GetStride() const { return stride; }
	};

	class VertexArray : public GraphicsObject
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const override;
		void Unbind() const override;
		void AddBuffer(const VertexBuffer* vb, const VertexBufferLayout& layout);
	};
}
