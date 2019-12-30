#pragma once

#include <string>
#include "Asset.h"

namespace Resources
{
	class ShaderAsset : public Asset
	{
	private:
		std::string* _data;
	public:
		ShaderAsset(const std::string& filePath);
		~ShaderAsset() override { delete _data; }
		void* GetData() const override { return _data; }
	};
}
