#pragma once

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

		Rendering::Renderer mRenderer;

		Systems::GameObject* mGameObject;
		Systems::GameObject* mGameObject2;
		Systems::Components::TransformComponent mCameraTransform
		{
			glm::vec3(75.0f, 125.0f, 150.0f),
			glm::vec3(30.0f, -30.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		};

	public:
		TestApp(float width, float height);
		~TestApp() override;
	protected:
		void LoadAssets() override;
		void InitializeGUI() override;
		void DeinitializeGUI() override;
		void UpdateGUI() override;
		void UpdateInput() override;
		void UpdateLogic() override;
		void UpdateScreen() override;
		void FinishFrame() override;
	private:
		void ShowTransform(const char* title, const bool collapsed, glm::vec3* position = nullptr,
			glm::vec3* rotation = nullptr, glm::vec3* scale = nullptr);
	};
}