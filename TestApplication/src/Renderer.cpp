#include <iostream>
#include "GL/glew.h"
#include "Renderer.h"

void GLClearError() { while (glGetError() != GL_NO_ERROR); }

bool GLLogCall(const char* function, const char* file, int line)
{
	while (auto error = glGetError())
	{
		std::cout << "[OpenGL] " << error << " in " << function << " " << file << ":" << line;
		return false;
	}
	return true;
}
