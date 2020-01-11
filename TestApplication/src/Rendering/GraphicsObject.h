#pragma once

namespace Rendering
{
	class GraphicsObject
	{
	protected:
		unsigned int _rendererId = 0;
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}
