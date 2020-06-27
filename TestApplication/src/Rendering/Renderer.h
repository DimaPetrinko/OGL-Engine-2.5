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
		float mWindowWidth;
		float mWindowHeight;
		static bool mIsInitialized;
		GLFWwindow* mWindow;
	public:
		Renderer(float windowWidth, float windowHeight);
		~Renderer();

		static bool IsInitialized() { return mIsInitialized; }
		static void SetWindowWidthAndHeight(Renderer* instance, const int width, const int height);
		float WindowWidth() const { return mWindowWidth; }
		float WindowHeight() const { return mWindowHeight; }
		GLFWwindow* GetWindow() const { return mWindow; }
		bool IsWindowClosed() const;
		void Clear() const;
		void Draw(const IndexBuffer& ib, const VertexArray& va, const Shader* sh) const;
		void PostRender() const;
	private:
		void UpdateProjectionMatrix();
		bool CreateWindow(GLFWwindow*& window, const char* title, int width, int height);
	};
}