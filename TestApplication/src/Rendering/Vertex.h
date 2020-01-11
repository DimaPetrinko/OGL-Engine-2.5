#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Rendering
{
	struct Vertex
	{
		glm::vec3 _position;
		glm::vec2 _texCoord;
		glm::vec3 _normal;
	};
}
