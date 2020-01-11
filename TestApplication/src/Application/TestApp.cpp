#include "TestApp.h"

namespace Application
{
	TestApp::TestApp(float windowWidth, float windowHeight) : _renderer(windowWidth, windowHeight)
	{
		if (_renderer.IsInitialized()) _running = true;
		else
		{
			printf("Could not initialize application\n");
			_running = false;
		}
	}

	TestApp::~TestApp()
	{
		delete _gameObject;
		delete _gameObject2;

		DeinitializeGUI();
	}

	bool TestApp::LoadAssets()
	{
		// for each gameobject - load its dependencies
		Rendering::Mesh* mesh;
		Rendering::Shader* shader;
		Rendering::Texture* texture;

		mesh = Resources::AssetDatabase::GetAsset<Rendering::Mesh>
			(WORKING_DIRECTORY "res/models/MonkeyHead.obj");
		shader = Resources::AssetDatabase::GetAsset<Rendering::Shader>
			(WORKING_DIRECTORY "res/shaders/Basic.shader");
		texture = Resources::AssetDatabase::GetAsset<Rendering::Texture>
			(WORKING_DIRECTORY "res/textures/checker3.jpg");
		_gameObject = new Systems::GameObject(&_renderer, mesh, texture, 1, shader);

		// to increment reference count
		mesh = Resources::AssetDatabase::GetAsset<Rendering::Mesh>
			(WORKING_DIRECTORY "res/models/cube.obj");
		shader = Resources::AssetDatabase::GetAsset<Rendering::Shader>
			(WORKING_DIRECTORY "res/shaders/Basic.shader");
		texture = Resources::AssetDatabase::GetAsset<Rendering::Texture>
			(WORKING_DIRECTORY "res/textures/image.png");
		_gameObject2 = new Systems::GameObject(&_renderer,mesh, texture, 2, shader);
		_gameObject2->Transform.Position = {0.0f, 0.0f, -100.0f};
		_gameObject2->Transform.Rotation = {0.0f, 45.0f, 0.0f};

		return true;
	}

	bool TestApp::InitializeGUI()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(_renderer.GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		return true;
	}

	bool TestApp::DeinitializeGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return true;
	}

	bool TestApp::UpdateGUI()
	{
		using namespace ImGui;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

		SetNextWindowPos(ImVec2(_renderer.WindowWidth(), 0.0f), 0, ImVec2(1.0f, 0.0f));
		SetNextWindowSize(ImVec2(350.0f, _renderer.WindowHeight()));

		if (Begin("Inspector", nullptr,
		ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove))
		{
			ShowTransform("Model", false, &_gameObject->Transform.Position,
				&_gameObject->Transform.Rotation, &_gameObject->Transform.Scale);
			ShowTransform("Model 2", false, &_gameObject2->Transform.Position,
				&_gameObject2->Transform.Rotation, &_gameObject2->Transform.Scale);

			Separator();
			Spacing();

			ShowTransform("Camera", false, &_cameraTransform.Position, &_cameraTransform.Rotation);

			SetNextItemOpen(true, ImGuiCond_Once);
			if (TreeNode("Metrics"))
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGui::Text("Frame time %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				TreePop();
			}

			End();
		}

		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		return true;
	}

	void TestApp::ShowTransform(const char* title, const bool collapsed, glm::vec3* position,
		glm::vec3* rotation, glm::vec3* scale)
	{
		using namespace ImGui;
		if (!collapsed) SetNextItemOpen(true, ImGuiCond_Once);
		if (TreeNode(title))
		{
			if (position != nullptr) SliderFloat3("Position", &position->x, -500.0f, 500.0f, "%.3f");
			if (rotation != nullptr) SliderFloat3("Rotation", &rotation->x, -360.0f, 360.0f, "%.3f");
			if (scale != nullptr) SliderFloat3("Scale", &scale->x, 0.0f, 5.0f, "%.3f");
			TreePop();
		}
	}

	bool TestApp::UpdateInput()
	{
		return true;
	}

	bool TestApp::UpdateLogic()
	{
		static const float deltaTime = 1.0f / 60.0f;

		_gameObject->Transform.Rotation.y += deltaTime * 20.0f;
		if (_gameObject->Transform.Rotation.y >= 360.0f) _gameObject->Transform.Rotation.y = 0.0f;

		return true;
	}

	bool TestApp::UpdateScreen()
	{
		if (_renderer.IsWindowClosed()) return false;

		_renderer.Clear();
		// move to Draw method of CameraRenderer
		_renderer.ViewMatrix = _cameraTransform.GetInvertedTranslationMatrix();

		_gameObject->Draw();
		_gameObject2->Draw();

		return true;
	}

	bool TestApp::FinishFrame()
	{
		_renderer.PostRender();
		return true;
	}
}