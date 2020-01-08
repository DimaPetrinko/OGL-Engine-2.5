#pragma once

#include <iostream>
#include "AssetManagement/AssetDatabase.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "TransformComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class GameObject
{
public:
	TransformComponent Transform;
private:
	const Rendering::Renderer* _renderer;

	Rendering::Mesh* _mesh = nullptr;
	Rendering::Texture* _texture = nullptr;
	Rendering::Shader* _shader = nullptr;
	unsigned int _textureSlot;
public:
	GameObject(const Rendering::Renderer* renderer, Rendering::Mesh* mesh,
		Rendering::Texture* texture, unsigned int textureSlot, Rendering::Shader* shader);
	~GameObject();
	void Draw();
};