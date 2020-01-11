#pragma once

#include "glm/glm.hpp"

namespace Rendering
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		
		bool operator==(const Vertex& other) const
		{
			return Position == other.Position
				&& TexCoord == other.TexCoord
				&& Normal == other.Normal;
		}
	};
}
