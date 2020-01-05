#include "TestApp.h"

namespace Application
{
	TestApp::TestApp(float windowWidth, float windowHeight) : _renderer(windowWidth, windowHeight),
		_vb(_renderer.IsInitialized()), _ib(_renderer.IsInitialized()), _va(_renderer.IsInitialized())
	{
		if (_renderer.IsInitialized()
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
		if (_shader != nullptr) Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_shader);
		if (_texture != nullptr) Resources::AssetDatabase::PutBack((Resources::Asset**)(void**)&_texture);
	}

	bool TestApp::UpdateInput()
	{
		return true;
	}

	bool TestApp::UpdateLogic()
	{
		static glm::vec3 step = glm::vec3(5.0f, 5.0f, 0.0f);
		static glm::vec3 direction = step;
		if (_position.x > _renderer.WindowWidth()) direction.x = -step.x;
		else if (_position.x < 0) direction.x = step.x;
		if (_position.y > _renderer.WindowHeight()) direction.y = -step.y;
		else if (_position.y < 0) direction.y = step.y;

		_position += direction;

		return true;
	}

	bool TestApp::UpdateScreen()
	{
		if (_renderer.IsWindowClosed()) return false;

		glm::vec3 cameraMovement(0.0f, 0.0f, 0.0f);

		glm::mat4 identityMat(1.0f);
		glm::mat4 modelMat = glm::translate(identityMat, _position);
		glm::mat4 viewMat = glm::translate(identityMat, -cameraMovement);
		glm::mat4 mvp = _renderer.ProjectionMatrix() * viewMat * modelMat;

		_shader->Bind();
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);

		_renderer.Clear();
		_renderer.Draw(_ib, _va, _shader);
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

	bool TestApp::SetUpGraphicsObjects()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] =
		{
			-50.0f, -50.0f,  0.0f,  0.0f,
			50.0f, -50.0f,  1.0f,  0.0f,
			50.0f,  50.0f,  1.0f,  1.0f,
			-50.0f,  50.0f,  0.0f,  1.0f
		};
		unsigned int indices[] = {0, 1, 2, 2, 3, 0};
		_vb.SetData(&positions[0], 4 * 4 * sizeof(float));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		_va.AddBuffer(_vb, layout);

		_ib.SetData(&indices[0], 6);

		_shader->Bind();
		int slot = 1;
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