#include "Renderer.h"

#include <iostream>
#include "Rendering.h"

namespace Rendering
{
	void GLClearError() { while (glGetError() != GL_NO_ERROR); }

	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (auto error = glGetError())
		{
			std::cout << "[OpenGL] " << error << " in " << function << " " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}

	bool Renderer::_isInitialized = false;

	Renderer::Renderer(float windowWidth, float windowHeight) : _windowWidth(windowWidth),
		 _windowHeight(windowHeight), _window(nullptr)
	{
		if (!CreateWindow(_window, "OGL-Engine-2.5", _windowWidth, _windowHeight))
		{
			_isInitialized = false;
		}
		auto error = glewInit();
		if (error != GLEW_OK)
		{
			std::cout << glewGetErrorString(error) << std::endl;
			_isInitialized = false;
		}
		_projectionMatrix =
			glm::perspectiveFov(20.0f, _windowWidth, _windowHeight, 0.0f, 100.0f);
			// glm::ortho(0.0f, _windowWidth, 0.0f, _windowHeight, -100.0f, 100.0f);
		printf("GL version: %s\n", glGetString(GL_VERSION));


		// GLCall(glEnable(GL_BLEND));
		// GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glFrontFace(GL_CW));
		GLCall(glCullFace(GL_BACK));
		GLCall(glEnable(GL_CULL_FACE));

		_isInitialized = true;
	}

	Renderer::~Renderer()
	{
		glfwDestroyWindow(_window);
	}

	bool Renderer::IsWindowClosed() const
	{
		return glfwWindowShouldClose(_window);
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Renderer::Draw(const IndexBuffer& ib, const VertexArray& va, const Shader* sh) const
	{
		sh->Bind();
		va.Bind();
		ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		ib.Unbind();
		va.Unbind();
		sh->Unbind();
	}

	void Renderer::PostRender() const
	{
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	bool Renderer::CreateWindow(GLFWwindow*& window, const char* title, int width, int height)
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

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		std::cout << "Created new window " << title << " (" << width << ", " << height << ")\n";
		return true;
	}
}
