#pragma once

#include <iostream>
#include "App.h"
#include "AssetManagement/AssetDatabase.h"
#include "Systems/GameObject.h"
#include "Rendering/Renderer.h"
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

		Systems::GameObject* _gameObject;
		Systems::GameObject* _gameObject2;
		Systems::Components::TransformComponent _cameraTransform
		{
			glm::vec3(75.0f, 125.0f, 150.0f),
			glm::vec3(30.0f, -30.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		};

	public:
		TestApp(float width, float height);
		~TestApp() override;
	protected:
		bool LoadAssets() override;
		bool InitializeGUI() override;
		bool DeinitializeGUI() override;
		bool UpdateGUI() override;
		bool UpdateInput() override;
		bool UpdateLogic() override;
		bool UpdateScreen() override;
		bool FinishFrame() override;
	private:
		void ShowTransform(const char* title, const bool collapsed, glm::vec3* position = nullptr,
			glm::vec3* rotation = nullptr, glm::vec3* scale = nullptr);
	};
}