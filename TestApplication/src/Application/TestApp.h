#pragma once

#include <iostream>
#include "App.h"
#include "AssetManagement/AssetDatabase.h"
#include "Rendering/GL.h"
#include "Rendering/Renderer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Application
{
	class TestApp : public App
	{
	private:
		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);

		Rendering::Renderer _renderer;
		Rendering::VertexBuffer _vb;
		Rendering::IndexBuffer _ib;
		Rendering::VertexArray _va;
		Rendering::Shader* _shader;
		Rendering::Texture* _texture;

	public:
		TestApp(float width, float height);
		~TestApp() override;
	protected:
		bool UpdateInput() override;
		bool UpdateLogic() override;
		bool UpdateScreen() override;
	private:
		bool LoadAssets();
		bool SetUpGraphicsObjects();
		bool UnbindAll();
	};
}