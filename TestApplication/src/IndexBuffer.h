#pragma once

#include "GraphicsObject.h"

class IndexBuffer final : public GraphicsObject
{
private:
	unsigned int count{};
public:
	~IndexBuffer();

	void Generate() override;
	void SetData(const unsigned int* data, const unsigned int count);
	void Bind() const override;
	void Unbind() const override;

	unsigned int GetCount() const { return count; }
};