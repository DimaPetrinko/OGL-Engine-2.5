#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define GL_INVALID_ID 0;
#define GLCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
};
