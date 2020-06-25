#include "Rendering/Mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils/ObjParser.h"

namespace Rendering
{
	void CalculateNormals(const Rendering::Triangle* triangles, unsigned int trianglesCount,
		Rendering::Vertex* vertices, unsigned int verticesCount)
	{
		for (unsigned int i = 0 ; i < trianglesCount ; i++)
		{
			unsigned int i0 = triangles[i].Indices[0];
			unsigned int i1 = triangles[i].Indices[1];
			unsigned int i2 = triangles[i].Indices[2];
			glm::vec3 v1 = vertices[i1].Position - vertices[i2].Position;
			glm::vec3 v2 = vertices[i0].Position - vertices[i2].Position;
			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);

			vertices[i2].Normal = normal;
		}

		for (unsigned int i = 0 ; i < verticesCount ; i++)
		{
			vertices[i].Normal = glm::normalize(vertices[i].Normal);
		}
	}

	Mesh::Mesh(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		if (stream.fail()) printf("%s doesn't exist\n", filePath);

		std::stringstream ss;
		std::string line;

		while (getline(stream, line)) ss << line << "\n";
		std::string data = std::string(ss.str());
		stream.close();

		Utils::ObjParser parser {};
		parser.Parse(data);

		std::cout << "Object: " << parser.Name << std::endl;
		std::cout << "\tVerts: " << parser.Vertices.size() << std::endl;
		std::cout << "\tTris: " << parser.Triangles.size() << std::endl;

		mName = parser.Name;

		new(&mIb) IndexBuffer(true);
		new(&mVb) VertexBuffer(true);
		new(&mVa) VertexArray(true);

		mIb.SetData(&parser.Triangles[0].Indices[0], parser.Triangles.size() * 3);
		mVb.SetData(&parser.Vertices[0], parser.Vertices.size() * sizeof(Rendering::Vertex));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		mVa.AddBuffer(mVb, layout);
	}

	Mesh::~Mesh()
	{
	}
}