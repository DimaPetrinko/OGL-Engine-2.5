#pragma once

#include "GraphicsObject.h"

namespace Rendering
{
	class IndexBuffer final : public GraphicsObject
	{
	private:
		unsigned int count{};
	public:
		IndexBuffer() {};
		IndexBuffer(const bool& glInitialized);
		~IndexBuffer();

		void SetData(const unsigned int* data, const unsigned int count);
		void Bind() const override;
		void Unbind() const override;

		inline unsigned int GetCount() const { return count; }
	};
}