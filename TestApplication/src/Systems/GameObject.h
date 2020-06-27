#pragma once

#include <iostream>
#include "AssetManagement/AssetDatabase.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Components/TransformComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Systems
{
	class GameObject
	{
	public:
		Components::TransformComponent Transform;
	private:
		const Rendering::Renderer* mRenderer;

		Rendering::Mesh* mMesh = nullptr;
		Rendering::Texture* mTexture = nullptr;
		Rendering::Shader* _shader = nullptr;
		unsigned int mTextureSlot;
	public:
		GameObject(const Rendering::Renderer* renderer, Rendering::Mesh* mesh,
			Rendering::Texture* texture, unsigned int textureSlot, Rendering::Shader* shader);
		~GameObject();
		void Draw();
	};
}