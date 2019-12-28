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
			std::cout << "[OpenGL] " << error << " in " << function << " " << file << ":" << line;
			return false;
		}
		return true;
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Renderer::Draw(const IndexBuffer* ib, const VertexArray* va, const Shader* sh) const
	{
		sh->Bind();
		va->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		ib->Unbind();
		va->Unbind();
		sh->Unbind();
	}
}
