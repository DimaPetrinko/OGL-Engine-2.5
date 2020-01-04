#include <iostream>
#include "Timer.h"
#include "Maths.h"
#include "AssetManagement/AssetDatabase.h"
#include "Rendering/Rendering.h"
#include "Rendering/Renderer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifdef PLATFORM_WIN64
#define WORKING_DIRECTORY ""
#elif PLATFORM_LINUX64
#define WORKING_DIRECTORY "TestApplication/"
#endif

const int EXITCODE_RUN = -1;
const int EXITCODE_EXIT = 0;
const int EXITCODE_ERROR = 1;

struct Vertex
{
	Vector2 position, textureCoord;

	Vertex() : position(Vector2(0.0f, 0.0f)), textureCoord(Vector2(0.0f, 0.0f)) {}
	Vertex(const Vector2& position, const Vector2& textureCoord)
		: position(position), textureCoord(textureCoord) {}
};

class Quad
{
private:
	Vector2 _position;
	unsigned int _indices[6];
	Vertex _vertices[4];
	Vertex _transformedVertices[4];

public:
	Quad(const Vertex& one, const Vertex& two, const Vertex& three, const Vertex& four)
		: _position(Vector2(0.0f, 0.0f)),
		_indices{0, 1, 2, 2, 3, 0},
		_vertices{ one, two, three, four },
		_transformedVertices{ one, two, three, four }
	{
		TransformVertices();
	}

	float* GetPositions()
	{
		return (float*)_transformedVertices;
	}

	Vector2 GetPosition() const
	{
		return _position;
	}

	unsigned int* GetIndices()
	{
		return _indices;
	}

	void Move(const Vector2& direction)
	{
		_position += direction;
		TransformVertices();
	}

private:
	void TransformVertices()
	{
		for (int i = 0; i < 4; i++)
		{
			_transformedVertices[i].position = _vertices[i].position + _position;
		}
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

	float _windowWidth = 480.0f;
	float _windowHeight = 320.0f;
	// glm::mat4 projMat = glm::ortho(-_windowWidth / 2, _windowWidth / 2,
			// -_windowHeight / 2, _windowHeight / 2, -1.0f, 1.0f);
	glm::mat4 projMat = glm::ortho(0.0f, _windowWidth, 0.0f, _windowHeight, 0.0f, 1.0f);

	Quad _quad;
	Rendering::Renderer* _renderer;
	Rendering::VertexBuffer* _vb;
	Rendering::IndexBuffer* _ib;
	Rendering::VertexArray* _va;
	Rendering::Shader* _shader;
	Rendering::Texture* _texture;
	Vector2 _step;
	Vector2 _direction;

public:
	TestApp() :	_quad(Quad(Vertex(Vector2(-50.0f, -50.0f), Vector2(0.0f, 0.0f)),
						   Vertex(Vector2(50.0f, -50.0f), Vector2(1.0f, 0.0f)),
						   Vertex(Vector2(50.0f, 50.0f), Vector2(1.0f, 1.0f)),
						   Vertex(Vector2(-50.0f, 50.0f), Vector2(0.0f, 1.0f)))),
				_step(Vector2(2.f, 0.5f)),
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

		_shader = Resources::AssetDatabase::GetAsset<Rendering::Shader>
		(WORKING_DIRECTORY "res/shaders/Basic.shader");
		_texture = Resources::AssetDatabase::GetAsset<Rendering::Texture>
		(WORKING_DIRECTORY "res/textures/image.png");

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		static float* positions = _quad.GetPositions();
		_vb->SetData(positions, 4 * 4 * sizeof(float));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		_va->AddBuffer(_vb, layout);

		_ib->SetData(_quad.GetIndices(), 6);

		_shader->Bind();
		int slot = 1;
		_texture->Bind(slot);
		_shader->SetUniform1i("u_Texture", slot);

		_vb->Unbind();
		_ib->Unbind();
		_va->Unbind();
		_texture->Unbind();
		_shader->Unbind();

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
		auto position = _quad.GetPosition();
		if (position.x > _windowWidth) _direction.x = -_step.x;
		else if (position.x < 0) _direction.x = _step.x;
		if (position.y > _windowHeight) _direction.y = -_step.y;
		else if (position.y < 0) _direction.y = _step.y;

		_quad.Move(_direction);

		return EXITCODE_RUN;
	}

	int UpdateScreen() override
	{
			Timer timer = Timer();
		// printf("UpdateScreen\n");
		if (glfwWindowShouldClose(_window)) return EXITCODE_EXIT;

		Vector2 position = _quad.GetPosition();
		glm::vec3 modelMovement(position.x, position.y, 0.0f);
		glm::vec3 cameraMovement(0.0f, 0.0f, 0.0f);

		glm::mat4 identityMat(1.0f);
		glm::mat4 modelMat = glm::translate(identityMat, modelMovement);
		glm::mat4 viewMat = glm::translate(identityMat, -cameraMovement);
		glm::mat4 mvp = projMat * viewMat * modelMat;

		_shader->Bind();
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
		// _shader->SetUniformMatrix4fv("u_M", &modelMat[0][0]);
		// _shader->SetUniformMatrix4fv("u_V", &viewMat[0][0]);
		// _shader->SetUniformMatrix4fv("u_P", &projMat[0][0]);
		_texture->Bind();

		_renderer->Clear();
		_renderer->Draw(_ib, _va, _shader);

		glfwPollEvents();
		glfwSwapBuffers(_window);
		return EXITCODE_RUN;
	}

	void Deinit() override
	{
		delete _vb;
		delete _ib;
		delete _va;
		delete _renderer;
		glfwDestroyWindow(_window);

		Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_shader);
		Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_texture);
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