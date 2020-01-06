#pragma once

#include <iostream>
#include "App.h"
#include "AssetManagement/AssetDatabase.h"
#include "GameObject.h"
#include "Rendering/GL.h"
#include "Rendering/Vertex.h"
#include "Rendering/Triangle.h"
#include "Rendering/Renderer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace Application
{
	class TestApp : public App
	{
	private:

		Rendering::Renderer _renderer;
		Rendering::VertexBuffer _vb;
		Rendering::IndexBuffer _ib;
		Rendering::VertexArray _va;
		Rendering::Shader* _shader;
		Rendering::Texture* _texture;

		GameObject* _gameObject;
		TransformComponent _cameraTransform
		{
			glm::vec3(0.0f, 0.0f, 300.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		};

	public:
		TestApp(float width, float height);
		~TestApp() override;
	protected:
		bool InitializeGUI() override;
		bool DeinitializeGUI() override;
		bool UpdateGUI() override;
		bool UpdateInput() override;
		bool UpdateLogic() override;
		bool UpdateScreen() override;
		bool FinishFrame() override;
	private:
		bool LoadAssets();
		bool SetUpGraphicsObjects();
		bool UnbindAll();
		void ShowTransform(const char* title, const bool collapsed, glm::vec3* position = nullptr,
			glm::vec3* rotation = nullptr, glm::vec3* scale = nullptr);
	};
}