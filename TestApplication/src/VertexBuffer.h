#pragma once

#include "GraphicsObject.h"

class VertexBuffer final : public GraphicsObject
{
public:
	~VertexBuffer();

	void Generate() override;
	void SetData(const void* data, const unsigned int size);
	void Bind() const override;
	void Unbind() const override;
};