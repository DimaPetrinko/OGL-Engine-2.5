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
		std::string mName {""};
		VertexBuffer mVb; // maybe move to renderer for batching?
		IndexBuffer mIb;
		VertexArray mVa;
	public:
		Mesh(const std::string& filePath);
		~Mesh();

		VertexArray* GetVA() { return &mVa; }
		IndexBuffer* GetIB() { return &mIb; }
	};
}