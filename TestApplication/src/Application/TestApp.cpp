#include "TestApp.h"

namespace Application
{
	TestApp::TestApp(float windowWidth, float windowHeight) : mRenderer(windowWidth, windowHeight)
	{
		if (!mRenderer.IsInitialized())
		{
			printf("Could not initialize application\n");
			mExitCode = EXIT_CODE_NOT_INITIALIZED;
		}
	}

	TestApp::~TestApp()
	{
		delete mGameObject;
		delete mGameObject2;

		DeinitializeGUI();
	}

	void TestApp::LoadAssets()
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
		mGameObject = new Systems::GameObject(&mRenderer, mesh, texture, 1, shader);

		// to increment reference count
		mesh = Resources::AssetDatabase::GetAsset<Rendering::Mesh>
			(WORKING_DIRECTORY "res/models/cube.obj");
		shader = Resources::AssetDatabase::GetAsset<Rendering::Shader>
			(WORKING_DIRECTORY "res/shaders/Basic.shader");
		texture = Resources::AssetDatabase::GetAsset<Rendering::Texture>
			(WORKING_DIRECTORY "res/textures/image.png");
		mGameObject2 = new Systems::GameObject(&mRenderer,mesh, texture, 2, shader);
		mGameObject2->Transform.Position = {0.0f, 0.0f, -100.0f};
		mGameObject2->Transform.Rotation = {0.0f, 45.0f, 0.0f};
	}

	void TestApp::InitializeGUI()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(mRenderer.GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void TestApp::DeinitializeGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void TestApp::UpdateGUI()
	{
		using namespace ImGui;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

		SetNextWindowPos(ImVec2(mRenderer.WindowWidth(), 0.0f), 0, ImVec2(1.0f, 0.0f));
		SetNextWindowSize(ImVec2(350.0f, mRenderer.WindowHeight()));

		if (Begin("Inspector", nullptr,
		ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove))
		{
			ShowTransform("Model", false, &mGameObject->Transform.Position,
				&mGameObject->Transform.Rotation, &mGameObject->Transform.Scale);
			ShowTransform("Model 2", false, &mGameObject2->Transform.Position,
				&mGameObject2->Transform.Rotation, &mGameObject2->Transform.Scale);

			Separator();
			Spacing();

			ShowTransform("Camera", false, &mCameraTransform.Position, &mCameraTransform.Rotation);

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

	void TestApp::UpdateInput()
	{
	}

	void TestApp::UpdateLogic()
	{
		static const float deltaTime = 1.0f / 60.0f;

		mGameObject->Transform.Rotation.y += deltaTime * 20.0f;
		if (mGameObject->Transform.Rotation.y >= 360.0f) mGameObject->Transform.Rotation.y = 0.0f;
	}

	void TestApp::UpdateScreen()
	{
		if (mRenderer.IsWindowClosed())
		{
			mExitCode = EXIT_CODE_NORMAL;
			return;
		}

		mRenderer.Clear();
		// move to Draw method of CameraRenderer
		mRenderer.ViewMatrix = mCameraTransform.GetInvertedTranslationMatrix();

		mGameObject->Draw();
		mGameObject2->Draw();
	}

	void TestApp::FinishFrame()
	{
		mRenderer.PostRender();
	}
}