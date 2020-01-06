#pragma once

#include <string>
#include <vector>
#include "AssetManagement/Asset.h"
#include "Rendering/Vertex.h"
#include "Rendering/Triangle.h"

namespace Rendering
{
	class Mesh : public Resources::Asset
	{
	private:
		std::vector<Vertex> _vertices; // change to std array?
		std::vector<Triangle> _triangles;
		unsigned int _verticesCount;
		unsigned int _trianglesCount;
	public:
		Mesh(const std::string& filePath);
		~Mesh();

		Vertex* GetVertices() { return &_vertices[0]; } // Dangerous! what if vector is realocated? the pointers
		Triangle* GetTriangles() { return &_triangles[0]; } // received with these methods now point to invalid data
		unsigned int GetVerticesCount() { return _verticesCount; }
		unsigned int GetTrianglesCount() { return _trianglesCount; }
		unsigned int GetIndicesCount() { return _trianglesCount * 3; }
	};
}