#pragma once

#include <vector>
#include "Renderer.h"
#include "GraphicsObject.h"
#include "VertexBuffer.h"

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
	template<typename T>
	void Push(const unsigned int& count) {}

	const std::vector<VertexBufferElement>& GetElements() const { return elements; }
	unsigned int GetStride() const { return stride; }
};


class VertexArray : public GraphicsObject
{
public:
	~VertexArray();

	void Generate() override;
	void Bind() const override;
	void Unbind() const override;
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};
