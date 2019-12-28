#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define GL_INVALID_ID 0;
#define GLCall(x) Rendering::GLClearError();\
	x;\
	Rendering::GLLogCall(#x, __FILE__, __LINE__)

namespace Rendering
{
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}