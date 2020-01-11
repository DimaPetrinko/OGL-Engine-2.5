#pragma once

#include <tuple>
#include <unordered_map>
#include "AssetManagement/Asset.h"
#include "GraphicsObject.h"

namespace Rendering
{
	class Shader : public GraphicsObject, public Resources::Asset
	{
	private:
		std::unordered_map<std::string, int> _uniformLocations;

	public:
		Shader(const std::string& filePath);
		~Shader();
		void Bind() const override;
		void Unbind() const override;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMatrix4fv(const std::string& name, float m[16]);

	private:
		std::tuple<std::string, std::string> ParseShader(const std::string* shaderData) const;
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name);
	};
}
