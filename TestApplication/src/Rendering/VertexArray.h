#pragma once

#include <vector>
#include "GL.h"
#include "GraphicsObject.h"
#include "VertexBuffer.h"

namespace Rendering
{
	struct VertexBufferElement
	{
		unsigned int Type;
		unsigned int Count;
		unsigned char Normalized;

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
		std::vector<VertexBufferElement> mElements;
		unsigned int mStride{};
	public:
		template <typename T>
		void Push(const unsigned int& count);

		inline const std::vector<VertexBufferElement>& GetElements() const { return mElements; }
		inline unsigned int GetStride() const { return mStride; }
	};

	class VertexArray : public GraphicsObject
	{
	public:
		VertexArray() {};
		VertexArray(const bool& glInitialized);
		~VertexArray();

		void Bind() const override;
		void Unbind() const override;
		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	};
}
