#include "Shader.h"

#include <fstream>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filePath) : _filePath(filePath) {}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_rendererId));
}

void Shader::Generate()
{
	auto[vertexShaderSource, fragmentShaderSource] = ParseShader(_filePath);
	_rendererId = CreateShader(vertexShaderSource, fragmentShaderSource);
}

void Shader::Bind() const
{
	GLCall(glUseProgram(_rendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string& name, float m[16])
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, m));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (_uniformLocations.find(name) != _uniformLocations.end()) return _uniformLocations[name];

	GLCall(int location = glGetUniformLocation(_rendererId, name.c_str()));
	if (location == -1) std::cout << "Uniform " << name << " doesn't exist\n";
	_uniformLocations[name] = location;
	return location;
}

std::tuple<std::string, std::string> Shader::ParseShader(const std::string& filePath) const
{
	std::ifstream stream(filePath);
	if (stream.fail()) printf("%s doesn't exist\n", filePath);

	enum class ShaderType
	{
		None = -1, Vertex = 0, Fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
		}
		else ss[(int)type] << line << "\n";
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		printf("Failed to compile shader! %s\n", message);
		GLCall(glDeleteShader(id));
		return GL_INVALID_ID;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}