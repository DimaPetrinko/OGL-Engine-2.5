#pragma once

#include <fstream>
#include "Asset.h"

namespace Resources
{
	class ShaderAsset : public Asset
	{
	private:
	public:
		ShaderAsset(const std::string& filePath)
		{
			std::ifstream stream(filePath);
			if (stream.fail()) printf("%s doesn't exist\n", filePath);

			stream.seekg(0, std::ios::end);
			auto size = stream.tellg();
			char* _data = new char[size];
			stream.seekg(0, std::ios::beg);
			stream.read(_data, size);
			stream.close();
		}
	};
}