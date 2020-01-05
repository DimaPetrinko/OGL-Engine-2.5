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
		static glm::vec3 step = glm::vec3(5.0f, 5.0f, 0.0f) / 4.0f;
		static glm::vec3 direction = step;
		if (_position.x > _renderer.WindowWidth()/3) direction.x = -step.x;
		else if (_position.x < 0) direction.x = step.x;
		if (_position.y > _renderer.WindowHeight()/3) direction.y = -step.y;
		else if (_position.y < 0) direction.y = step.y;

		_position += direction;

		return true;
	}

	bool TestApp::UpdateScreen()
	{
		if (_renderer.IsWindowClosed()) return false;

		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		// glm::vec3 cameraMovement(_renderer.WindowWidth(), _renderer.WindowHeight(), 700.0f);
		glm::vec3 cameraMovement(200.0f, 150.0f, 300.0f);
		// glm::vec3 cameraMovement(0.0f, 0.0f, 0.0f);

		glm::mat4 identityMat(1.0f);

		glm::mat4 t = glm::translate(identityMat, _position);
		glm::mat4 rX = glm::rotate(identityMat, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rY = glm::rotate(identityMat, 0.0f/*3.14f / 2*/, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rZ = glm::rotate(identityMat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 r = rX * rY * rZ;
		glm::mat4 s = glm::scale(identityMat, scale);

		glm::mat4 modelMat = t * r * s;
		glm::mat4 viewMat = glm::translate(identityMat, -cameraMovement);
		glm::mat4 mvp = _renderer.ProjectionMatrix() * viewMat * modelMat;

		_shader->Bind();
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
		_shader->SetUniformMatrix4fv("u_M", &modelMat[0][0]);

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

	struct Vertex
	{
		glm::vec3 _position;
		glm::vec2 _texCoord;
		glm::vec3 _normal;
	};

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
		unsigned int indicesCount = 3 * 3;
		unsigned int indices[] = {3, 0, 2, 0, 3, 1, 2, 1, 3};
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