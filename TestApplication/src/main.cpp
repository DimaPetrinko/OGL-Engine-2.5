#include <iostream>
#include "Maths.h"
#include "AssetManagement/AssetDatabase.h"
#include "AssetManagement/ShaderAsset.h"
#include "Rendering/Rendering.h"
#include "Rendering/Renderer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Shader.h"

#ifdef PLATFORM_WIN64
#define WORKING_DIRECTORY ""
#elif PLATFORM_LINUX64
#define WORKING_DIRECTORY "TestApplication/"
#endif

const int EXITCODE_RUN = -1;
const int EXITCODE_EXIT = 0;
const int EXITCODE_ERROR = 1;

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
	GLFWwindow* _window = nullptr;
private:
	int _exitCode {};
public:
	virtual ~App() = default;
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
		Deinit();
		std::cout<< "Exited with code " << _exitCode<<std::endl;
		return _exitCode;
	}
protected:
	virtual int Init() = 0;
	virtual int UpdateInput() = 0;
	virtual int UpdateLogic() = 0;
	virtual int UpdateScreen() = 0;
	virtual void Deinit() = 0;
};

class TestApp final : public App
{
private:

	int _windowWidth = 480;
	int _windowHeight = 320;

	Quad _quad;
	Rendering::Renderer* _renderer;
	Rendering::VertexBuffer* _vb;
	Rendering::IndexBuffer* _ib;
	Rendering::VertexArray* _va;
	Rendering::Shader* _basicShader;
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
		if (!CreateWindow("OGL-Engine-2.5", _windowWidth, _windowHeight)) return EXITCODE_ERROR;

		auto error = glewInit();
		if (error != GLEW_OK)
		{
			std::cout << glewGetErrorString(error) << std::endl;
			return EXITCODE_ERROR;
		}
		printf("GL version: %s\n", glGetString(GL_VERSION));

		_renderer = new Rendering::Renderer();
		_vb = new Rendering::VertexBuffer();
		_va = new Rendering::VertexArray();
		_ib = new Rendering::IndexBuffer();

		Resources::ShaderAsset* shaderAsset = Resources::AssetDatabase
		::GetAsset<Resources::ShaderAsset>(WORKING_DIRECTORY "res/shaders/Basic.shader");
		_basicShader = new Rendering::Shader(shaderAsset);

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(2);
		_va->AddBuffer(_vb, layout);

		_ib->SetData(_quad.GetIndices(), 6);

		_basicShader->Unbind();

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
		if (_quad.position.x > _windowWidth) _direction.x = -_step.x;
		else if (_quad.position.x < 0) _direction.x = _step.x;
		if (_quad.position.y > _windowHeight) _direction.y = -_step.y;
		else if (_quad.position.y < 0) _direction.y = _step.y;

		_quad.Move(_direction);

		return EXITCODE_RUN;
	}

	int UpdateScreen() override
	{
		// printf("UpdateScreen\n");
		if (glfwWindowShouldClose(_window)) return EXITCODE_EXIT;

		static float* positions = _quad.GetPositions();
		static unsigned int* indices = _quad.GetIndices();
		Vector2 positionNormalized = _quad.position.Normalized();

		float mvpMatrix[16] =
		{
			2.0f/_windowWidth, 0, 0, -1,
			0, 2.0f/_windowHeight, 0, -1,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};

		_vb->SetData(positions, 8 * sizeof(float));

		_basicShader->Bind();
		_basicShader->SetUniform4f("u_color", positionNormalized.x, 10.0f, positionNormalized.y, 1.0f);
		_basicShader->SetUniformMatrix4fv("u_mvp", mvpMatrix);

		_renderer->Clear();
		_renderer->Draw(_ib, _va, _basicShader);

		glfwPollEvents();
		glfwSwapBuffers(_window);
		return EXITCODE_RUN;
	}

	void Deinit() override
	{
		delete _vb;
		delete _ib;
		delete _va;
		delete _basicShader;
		delete _renderer;
		glfwDestroyWindow(_window);

		Resources::AssetDatabase::UnloadAllAssets();

		printf("Deinit\n");
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
};

int main()
{	
	App* app = new TestApp();
	
	int exitCode = app->Run();
	
	delete app;
	return exitCode;
}