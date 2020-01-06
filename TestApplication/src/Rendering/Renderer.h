#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace Rendering
{
	class Renderer
	{
	public:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
	private:

		float _windowWidth;
		float _windowHeight;
		static bool _isInitialized;
		GLFWwindow* _window;
	public:
		Renderer(float windowWidth, float windowHeight);
		~Renderer();

		static bool IsInitialized() { return _isInitialized; }
		float WindowWidth() const { return _windowWidth; }
		float WindowHeight() const { return _windowHeight; }
		GLFWwindow* GetWindow() const { return _window; }
		bool IsWindowClosed() const;
		void Clear() const;
		void Draw(const IndexBuffer& ib, const VertexArray& va, const Shader* sh) const;
		void PostRender() const;
	private:
		bool CreateWindow(GLFWwindow*& window, const char* title, int width, int height);
	};
}