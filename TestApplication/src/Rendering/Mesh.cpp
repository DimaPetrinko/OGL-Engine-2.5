#include "Rendering/Mesh.h"

namespace Rendering
{
	void CalculateNormals(const Rendering::Triangle* triangles, unsigned int trianglesCount,
		Rendering::Vertex* vertices, unsigned int verticesCount)
	{
		for (unsigned int i = 0 ; i < trianglesCount ; i++)
		{
			unsigned int i0 = triangles[i].indices[0];
			unsigned int i1 = triangles[i].indices[1];
			unsigned int i2 = triangles[i].indices[2];
			glm::vec3 v1 = vertices[i1]._position - vertices[i0]._position;
			glm::vec3 v2 = vertices[i2]._position - vertices[i0]._position;
			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);

			vertices[i0]._normal = normal;
		}

		for (unsigned int i = 0 ; i < verticesCount ; i++)
		{
			vertices[i]._normal = glm::normalize(vertices[i]._normal);
		}
	}

	Mesh::Mesh(const std::string& filePath)
	{
		// load from file
		// parse data into vertices and triangles

		//Test
		unsigned int trianglesCount = 4;
		Triangle triangles[] =
		{
			{2, 0, 1},
			{3, 1, 0},
			{1, 3, 2},
			{0, 2, 3}
		};

		unsigned int verticesCount = 4;
		Vertex vertices[] =
		{
			{glm::vec3(-50.0f, 0.0f,  0.0f),  glm::vec2(0.0f, 0.0f)},
			{glm::vec3( 50.0f, 0.0f,  0.0f),  glm::vec2(1.0f, 0.0f)},
			{glm::vec3( 0.0f,  50.0f, 0.0f),  glm::vec2(0.5f, 1.0f)},
			{glm::vec3( 0.0f,  25.0f, 50.0f), glm::vec2(0.5f, 0.5f)}
		};

		new(&_ib) IndexBuffer(true);
		new(&_vb) VertexBuffer(true);
		new(&_va) VertexArray(true);

		_ib.SetData(&triangles[0].indices[0], trianglesCount * 3);
		//calculate normals?
		CalculateNormals(triangles,trianglesCount, vertices, verticesCount);
		_vb.SetData(&vertices[0], verticesCount * sizeof(Rendering::Vertex));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		_va.AddBuffer(_vb, layout);
	}

	Mesh::~Mesh()
	{
		// clean up
	}
}