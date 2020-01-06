#include "TestApp.h"

namespace Application
{
	TestApp::TestApp(float windowWidth, float windowHeight) : _renderer(windowWidth, windowHeight),
		_vb(_renderer.IsInitialized()), _ib(_renderer.IsInitialized()), _va(_renderer.IsInitialized())
	{
		if (_renderer.IsInitialized()
		&& InitializeGUI()
		&& LoadAssets()
		&& SetUpGraphicsObjects()
		&& UnbindAll()) _running = true;
		else
		{
			printf("Could not initialize application\n");
			_running = false;
		}
	}

	TestApp::~TestApp()
	{
		DeinitializeGUI();
		if (_shader != nullptr) Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_shader);
		if (_texture != nullptr) Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_texture);
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

		SetNextWindowPos(ImVec2(_renderer.WindowWidth(), 0.0f), ImGuiCond_Once, ImVec2(1.0f, 0.0f));
		SetNextWindowSize(ImVec2(350.0f, _renderer.WindowHeight()), ImGuiCond_Once);

		if (Begin("Inspector", nullptr,
		ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove))

		{
			ShowTransform("Model", false, &_modelPosition, &_modelRotation, &_modelScale);

			Separator();
			Spacing();

			ShowTransform("Camera", false, &_cameraPosition, &_cameraRotation);

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
			if (scale != nullptr) SliderFloat3("Scale", &scale->x, -5.0f, 5.0f, "%.3f");
			TreePop();
		}
	}

	bool TestApp::UpdateInput()
	{
		return true;
	}

	bool TestApp::UpdateLogic()
	{
		// static glm::vec3 step = glm::vec3(5.0f, 5.0f, 0.0f) / 4.0f;
		// static glm::vec3 direction = step;
		// if (_position.x > _renderer.WindowWidth()/3) direction.x = -step.x;
		// else if (_position.x < 0) direction.x = step.x;
		// if (_position.y > _renderer.WindowHeight()/3) direction.y = -step.y;
		// else if (_position.y < 0) direction.y = step.y;

		// _position += direction;

		return true;
	}

	bool TestApp::UpdateScreen()
	{
		if (_renderer.IsWindowClosed()) return false;

		glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

		auto modelRadians = glm::radians(_modelRotation);
		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, _modelPosition);
		modelMat = glm::rotate(modelMat, modelRadians.x, xAxis);
		modelMat = glm::rotate(modelMat, modelRadians.y, yAxis);
		modelMat = glm::rotate(modelMat, modelRadians.z, zAxis);
		modelMat = glm::scale(modelMat, _modelScale);

		auto viewRadians = glm::radians(_cameraRotation);
		glm::mat4 viewMat(1.0f);
		viewMat = glm::rotate(viewMat, viewRadians.x, xAxis);
		viewMat = glm::rotate(viewMat, viewRadians.y, yAxis);
		viewMat = glm::rotate(viewMat, viewRadians.z, zAxis);
		viewMat = glm::translate(viewMat, -_cameraPosition);

		glm::mat4 mvp = _renderer.ProjectionMatrix() * viewMat * modelMat;

		_shader->Bind();
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
		_shader->SetUniformMatrix4fv("u_M", &modelMat[0][0]);

		_renderer.Clear();
		_renderer.Draw(_ib, _va, _shader);

		return true;
	}

	bool TestApp::FinishFrame()
	{
		_renderer.PostRender();
		return true;
	}

	bool TestApp::LoadAssets()
	{
		_shader = Resources::AssetDatabase::GetAsset<Rendering::Shader>
		(WORKING_DIRECTORY "res/shaders/Basic.shader");
		_texture = Resources::AssetDatabase::GetAsset<Rendering::Texture>
		(WORKING_DIRECTORY "res/textures/image.png");
		return true;
	}

	struct Vertex
	{
		glm::vec3 _position;
		glm::vec2 _texCoord;
		glm::vec3 _normal;
	};

	// move to renderer?
	void CalculateNormals(const unsigned int* indices, unsigned int indicesCount,
		Vertex* vertices, unsigned int verticesCount)
	{
		for (unsigned int i = 0 ; i < indicesCount ; i += 3)
		{
			unsigned int i0 = indices[i];
			unsigned int i1 = indices[i + 1];
			unsigned int i2 = indices[i + 2];
			glm::vec3 v1 = vertices[i1]._position - vertices[i0]._position;
			glm::vec3 v2 = vertices[i2]._position - vertices[i0]._position;
			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);

			vertices[i0]._normal = normal;
		}

		for (unsigned int i = 0 ; i < verticesCount ; i++)
		{
			vertices[i]._normal = glm::normalize(vertices[i]._normal);
		}
	}

	bool TestApp::SetUpGraphicsObjects()
	{
		unsigned int indicesCount = 4 * 3; // n triangles
		unsigned int indices[] =
		{
			2, 0, 1,
			3, 1, 0,
			1, 3, 2,
			0, 2, 3
		};
		_ib.SetData(&indices[0], indicesCount);

		unsigned int verticesCount = 4;
		Vertex vertices[] =
		{
			Vertex {glm::vec3(-50.0f, 0.0f,  0.0f),  glm::vec2(0.0f, 0.0f)},
			Vertex {glm::vec3( 50.0f, 0.0f,  0.0f),  glm::vec2(1.0f, 0.0f)},
			Vertex {glm::vec3( 0.0f,  50.0f, 0.0f),  glm::vec2(0.5f, 1.0f)},
			Vertex {glm::vec3( 0.0f,  25.0f, 50.0f), glm::vec2(0.5f, 0.5f)}
		};
		CalculateNormals(indices, indicesCount, vertices, verticesCount);
		_vb.SetData(&vertices[0], verticesCount * sizeof(Vertex));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		_va.AddBuffer(_vb, layout);

		int slot = 1;
		_shader->Bind();
		_texture->Bind(slot);
		_shader->SetUniform1i("u_Texture", slot);

		return true;
	}

	bool TestApp::UnbindAll()
	{
		_vb.Unbind();
		_ib.Unbind();
		_va.Unbind();
		_shader->Unbind();
		return true;
	}
}