#pragma once

#include <string>
#include "GraphicsObject.h"
#include "GL.h"
#include "AssetManagement/Asset.h"

namespace Rendering
{
	class Texture : public GraphicsObject, public Resources::Asset
	{
	private:
		int _width, _height, _BPP;
	public:
		Texture(const std::string& filePath);
		~Texture();

		void Bind() const override;
		void Bind(const unsigned int& slot) const;
		void Unbind() const override;
	};
}