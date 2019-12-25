#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include "Math.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#ifdef PLATFORM_WIN64
#define WORKING_DIRECTORY ""
#elif PLATFORM_LINUX64
#define WORKING_DIRECTORY "TestApplication/"
#endif

const int EXITCODE_RUN = -1;

class Quad
{
public:
	Vector2 position;

private:
	unsigned int _indices[6];
	Vector2 _vertices[4];
	Vector2 _transformedVertices[4];

public:
	Quad(const Vector2& one, const Vector2& two, const Vector2& three, const Vector2& four)
		: position(Vector2(0.0f, 0.0f)),
		_indices{0, 1, 2, 2, 3, 0},
		_vertices{ one, two, three, four },
		_transformedVertices{ Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) }
	{
		TransformVertices();
	}

	float* GetPositions()
	{
		return (float*)(Vector2*)_transformedVertices;
	}

	unsigned int* GetIndices()
	{
		return _indices;
	}

	void Move(const Vector2& direction)
	{
		position += direction;
		TransformVertices();
	}

private:
	void TransformVertices()
	{
		for (int i = 0; i < 4; i++) _transformedVertices[i] = _vertices[i] + position;
	}
};

class App
{
protected:
	GLFWwindow* _window;
private:
	int _exitCode;
public:
	int Run()
	{
		_exitCode = Init();

		while (true)
		{
			if (_exitCode != EXITCODE_RUN) break;
			if ((_exitCode = UpdateInput()) != EXITCODE_RUN) break;
			if ((_exitCode = UpdateLogic()) != EXITCODE_RUN) break;
			if ((_exitCode = UpdateScreen()) != EXITCODE_RUN) break;
		}
		std::cout<<_exitCode<<std::endl;
		Deinit();
		return _exitCode;
	}
protected:
	virtual int Init() = 0;
	virtual int UpdateInput() = 0;
	virtual int UpdateLogic() = 0;
	virtual int UpdateScreen() = 0;
	virtual void Deinit() = 0;
};

class TestApp : public App
{
private:

	int _windowWidth = 480;
	int _windowHeight = 320;

	Quad _quad;
	VertexBuffer _vb{};
	IndexBuffer _ib{};
	VertexArray _va{};
	unsigned int _basicShader{};
	int _colorUniformLocation{};
	int _mvpUniformLocation{};
	Vector2 _step;
	Vector2 _direction;

public:
	TestApp() : _quad(Quad(Vector2(-50.0f, -50.0f),
						  Vector2(50.0f, -50.0f),
						  Vector2(50.0f, 50.0f),
						  Vector2(-50.0f, 50.0f))),
				_step(Vector2(20.f, 5.f)),
				_direction(Vector2(_step.x, _step.y)){}

protected:
	int Init() override
	{
		if (!CreateWindow("OGL-Engine-2.5", _windowWidth, _windowHeight)) return 1;

		auto error = glewInit();
		if (error != GLEW_OK)
		{
			std::cout << glewGetErrorString(error) << std::endl;
			return 1;
		}
		printf("GL version: %s\n", glGetString(GL_VERSION));

		_ib.Generate();
		_vb.Generate();
		_va.Generate();

		_va.Bind();
		VertexBufferLayout layout;
		layout.Push<float>(2);
		_va.AddBuffer(_vb, layout);
		_va.Unbind();

		_ib.Bind();
		_ib.SetData(_quad.GetIndices(), 6);
		_ib.Unbind();

		// glVertexAttribPointer(attribute index, elements count, GL_FLOAT, normalize (for 0 .. 255 byte or smth),
		// size of vertex in bytes (includes texture coords), starting index (in bytes));

		GLCall(glUseProgram(0));

		auto[vertexShaderSource, fragmentShaderSource] = ParseShader(WORKING_DIRECTORY "res/shaders/Basic.shader");
		_basicShader = CreateShader(vertexShaderSource, fragmentShaderSource);
		GLCall(_colorUniformLocation = glGetUniformLocation(_basicShader, "u_color"));
		GLCall(_mvpUniformLocation = glGetUniformLocation(_basicShader, "u_mvp"));

		return EXITCODE_RUN;
	}

	int UpdateInput() override
	{
		// printf("UpdateInput\n");
		return EXITCODE_RUN;
	}

	int UpdateLogic() override
	{
		// printf("UpdateLogic\n");
		return EXITCODE_RUN;
	}

	int UpdateScreen() override
	{
		// printf("UpdateScreen\n");
		if (glfwWindowShouldClose(_window)) return 0;

		const auto w = (float)_windowWidth;
		const auto h = (float)_windowHeight;

		if (_quad.position.x > w) _direction.x = -_step.x;
		else if (_quad.position.x < 0) _direction.x = _step.x;
		if (_quad.position.y > h) _direction.y = -_step.y;
		else if (_quad.position.y < 0) _direction.y = _step.y;

		_quad.Move(_direction);

		static float* positions = _quad.GetPositions();
		static unsigned int* indices = _quad.GetIndices();
		Vector2 positionNormalized = _quad.position.Normalized();

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(_basicShader));
		GLCall(glUniform4f(_colorUniformLocation, positionNormalized.x, 10.0f, positionNormalized.y, 1.0f));

		float mvpMatrix[16] =
		{
			2/w,0,	0,	-1,
			0,	2/h,0,	-1,
			0,	0,	1,	0,
			0,	0,	0,	 1,
		};
		GLCall(glUniformMatrix4fv(_mvpUniformLocation, 1, GL_TRUE, mvpMatrix));

		_vb.Bind();
		_vb.SetData(positions, 8 * sizeof(float));
		_vb.Unbind();

		_va.Bind();
		_ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		glfwPollEvents();
		glfwSwapBuffers(_window);
		return EXITCODE_RUN;
	}

	void Deinit() override
	{
		printf("Deinit\n");
		GLCall(glDeleteProgram(_basicShader));
		glfwDestroyWindow(_window);
	}

private:
	bool CreateWindow(const char* title, int width, int height)
	{
		static bool glfwInitialized;
		if (!glfwInitialized)
		{
			auto success = glfwInit();
			// TODO: make a macro
			if (!success)
			{
				printf("Could not initialize GLFW!\n");
				return false;
			}
			glfwInitialized = true;
			printf("Initialized GLFW. Version: %s\n", glfwGetVersionString());
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(_window);
		glfwSwapInterval(1);

		std::cout << "Created new window " << title << " (" << width << ", " << height << ")\n";
		return true;
	}

	static std::tuple<std::string, std::string> ParseShader(const std::string& filePath)
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

	static unsigned int CompileShader(unsigned int type, const std::string& source)
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
			return 0;
		}

		return id;
	}

	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
};

int main()
{
	App* app = new TestApp();

	int exitCode = app->Run();

	delete(app);
	return exitCode;
}