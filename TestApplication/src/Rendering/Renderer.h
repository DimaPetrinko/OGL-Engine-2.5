#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Rendering
{
	class Renderer
	{
	private:
	public:
		Renderer() {};
		~Renderer() {};

		void Clear() const;
		void Draw(const IndexBuffer* ib, const VertexArray* va, const Shader* sh) const;
	};
}