#include "Renderer.h"

#include <iostream>
#include "GL.h"

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

	void FramebuferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Renderer::Renderer::SetWindowWidthAndHeight(nullptr, width, height);
	}

	void Renderer::SetWindowWidthAndHeight(Renderer* instance, const int width, const int height)
	{
		static Renderer* renderer;
		if (instance) renderer = instance;
		renderer->_windowWidth = width;
		renderer->_windowHeight = height;
		renderer->UpdateProjectionMatrix();
		GLCall(glViewport(0, 0, width - 350.0f, height));
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
		printf("GL version: %s\n", glGetString(GL_VERSION));

		UpdateProjectionMatrix();

		glClearColor(0.05f, 0.05f, 0.15f, 1.0f);

		glfwSetFramebufferSizeCallback(_window, &FramebuferSizeCallback);
		int currentWidth, currentHeight;
		glfwGetFramebufferSize(_window, &currentWidth, &currentHeight);
		SetWindowWidthAndHeight(this, currentWidth, currentHeight);

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		// GLCall(glEnable(GL_BLEND));
		// GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glFrontFace(GL_CCW));
		GLCall(glCullFace(GL_BACK));
		GLCall(glEnable(GL_CULL_FACE));

		_isInitialized = true;
	}

	Renderer::~Renderer()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	bool Renderer::IsWindowClosed() const
	{
		return glfwWindowShouldClose(_window);
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

	void Renderer::UpdateProjectionMatrix()
	{
		ProjectionMatrix = glm::perspectiveFov(20.0f, _windowWidth - 350.0f, _windowHeight, 0.01f, 1000.0f);
			// glm::ortho(0.0f, _windowWidth, 0.0f, _windowHeight, -100.0f, 100.0f);
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

		// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
