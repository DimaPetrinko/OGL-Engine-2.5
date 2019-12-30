#include "ShaderAsset.h"
#include <fstream>
#include <string>
#include <sstream>

namespace Resources
{
	ShaderAsset::ShaderAsset(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		if (stream.fail()) printf("%s doesn't exist\n", filePath);

		std::stringstream ss;
		std::string line;

		while (getline(stream, line))
		{
			ss << line << "\n";
		}
		_data = new std::string(ss.str());
	}
}
