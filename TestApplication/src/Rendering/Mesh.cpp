#include "Rendering/Mesh.h"

namespace Rendering
{
	Mesh::Mesh(const std::string& filePath)
	{
		// load from file
		// parse data into vertices and triangles

		//Test
		_trianglesCount = 4;
		_triangles =
		{
			{2, 0, 1},
			{3, 1, 0},
			{1, 3, 2},
			{0, 2, 3}
		};

		_verticesCount = 4;
		_vertices =
		{
			{glm::vec3(-50.0f, 0.0f,  0.0f),  glm::vec2(0.0f, 0.0f)},
			{glm::vec3( 50.0f, 0.0f,  0.0f),  glm::vec2(1.0f, 0.0f)},
			{glm::vec3( 0.0f,  50.0f, 0.0f),  glm::vec2(0.5f, 1.0f)},
			{glm::vec3( 0.0f,  25.0f, 50.0f), glm::vec2(0.5f, 0.5f)}
		};
	}

	Mesh::~Mesh()
	{
		// clean up
	}
}