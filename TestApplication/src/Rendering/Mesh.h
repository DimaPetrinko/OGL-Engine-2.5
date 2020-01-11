#pragma once

#include <string>
#include "AssetManagement/Asset.h"
#include "Rendering/Vertex.h"
#include "Rendering/Triangle.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"

namespace Rendering
{
	class Mesh : public Resources::Asset
	{
	private:
		std::string _name {""};
		VertexBuffer _vb; // maybe move to renderer for batching?
		IndexBuffer _ib;
		VertexArray _va;
	public:
		Mesh(const std::string& filePath);
		~Mesh();

		VertexArray* GetVA() { return &_va; }
		IndexBuffer* GetIB() { return &_ib; }
	};
}