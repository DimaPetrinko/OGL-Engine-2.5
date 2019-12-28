#pragma once

#include "GraphicsObject.h"

namespace Rendering
{
	class VertexBuffer final : public GraphicsObject
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void SetData(const void* data, const unsigned int size);
		void Bind() const override;
		void Unbind() const override;
	};
}