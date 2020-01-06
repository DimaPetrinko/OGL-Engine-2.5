#pragma once

#include <iostream>
#include "AssetManagement/AssetDatabase.h"
#include "Rendering/Renderer.h"
#include "Rendering/Vertex.h"
#include "Rendering/Triangle.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct TransformComponent
{
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);


	static const glm::vec3 xAxis;
	static const glm::vec3 yAxis;
	static const glm::vec3 zAxis;

	glm::mat4 GetMatrix()
	{
		auto radians = glm::radians(Rotation);
		glm::mat4 transformation(1.0f);
		transformation = glm::translate(transformation, Position);
		transformation = glm::rotate(transformation, radians.z, zAxis);
		transformation = glm::rotate(transformation, radians.x, xAxis);
		transformation = glm::rotate(transformation, radians.y, yAxis);
		transformation = glm::scale(transformation, Scale);
		return transformation;
	}

	glm::mat4 GetInvertedTranslationMatrix()
	{
		auto radians = glm::radians(Rotation);
		glm::mat4 transformation(1.0f);
		transformation = glm::rotate(transformation, radians.z, zAxis);
		transformation = glm::rotate(transformation, radians.x, xAxis);
		transformation = glm::rotate(transformation, radians.y, yAxis);
		transformation = glm::translate(transformation, -Position);
		return transformation;
	}
};

class GameObject
{
public:
	TransformComponent Transform;
private:
	glm::vec3 _modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 _cameraPosition = glm::vec3(0.0f, 0.0f, 300.0f);
	glm::vec3 _cameraRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	const Rendering::Renderer* _renderer;

	Rendering::Mesh* _mesh = nullptr;
	Rendering::Texture* _texture = nullptr;
	Rendering::Shader* _shader = nullptr;

	Rendering::VertexBuffer _vb; // maybe move to renderer for batching?
	Rendering::IndexBuffer _ib;
	Rendering::VertexArray _va;
public:
	void CalculateNormals(const Rendering::Triangle* triangles, unsigned int trianglesCount,
		Rendering::Vertex* vertices, unsigned int verticesCount)
	{
		for (unsigned int i = 0 ; i < trianglesCount ; i++)
		{
			unsigned int i0 = triangles[i].indices[0];
			unsigned int i1 = triangles[i].indices[1];
			unsigned int i2 = triangles[i].indices[2];
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

	GameObject(const Rendering::Renderer* renderer, Rendering::Mesh* mesh,
		Rendering::Texture* texture, Rendering::Shader* shader)
			: _renderer(renderer), _mesh(mesh),
			  _texture(texture), _shader(shader)
	{
		if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
		{
			std::cout << "One of assets is null. Cannot initialize!\n";
			return;
		}

		_ib.~IndexBuffer();
		new(&_ib) Rendering::IndexBuffer(true); // Calls destrutor right away
		_vb.~VertexBuffer();
		new(&_vb) Rendering::VertexBuffer(true);
		_va.~VertexArray();
		new(&_va) Rendering::VertexArray(true);

		Rendering::Triangle* triangles =
		_mesh->GetTriangles();
		// {
		// 	{2, 0, 1},
		// 	{3, 1, 0},
		// 	{1, 3, 2},
		// 	{0, 2, 3}
		// };
		Rendering::Vertex* vertices =
		 _mesh->GetVertices();
		// {
		// 	{glm::vec3(-50.0f, 0.0f,  0.0f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		// 	{glm::vec3( 50.0f, 0.0f,  0.0f),  glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		// 	{glm::vec3( 0.0f,  50.0f, 0.0f),  glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		// 	{glm::vec3( 0.0f,  25.0f, 50.0f), glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)}
		// };

		// ===================================================
		unsigned int* p = &triangles[0].indices[0];
		for (int i = 0; i < _mesh->GetIndicesCount(); i++)
		{
			std::cout << p[i] << ", ";
		}
		std::cout << std::endl;
		float* v = (float*)&vertices[0];
		for (int i = 0; i < _mesh->GetVerticesCount() * 8; i++)
		{
			std::cout << v[i] << ", ";
		}
		// ===================================================

		_ib.SetData(&triangles[0].indices[0], _mesh->GetIndicesCount());
		//calculate normals?
		CalculateNormals(triangles, _mesh->GetTrianglesCount(), vertices, _mesh->GetVerticesCount());
		_vb.SetData(&vertices[0], _mesh->GetVerticesCount() * sizeof(Rendering::Vertex));

		Rendering::VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		_va.AddBuffer(_vb, layout);

		int slot = 1; // querry a free slot with global function?
		_texture->Bind(slot);
		_shader->Bind();
		_shader->SetUniform1i("u_Texture", slot);
	}

	~GameObject()
	{
		if (_mesh != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_mesh));
		if (_texture != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_texture));
		if (_shader != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_shader));
	}

	void Draw()
	{
		glm::mat4 modelMat = Transform.GetMatrix();

		glm::mat4 mvp = _renderer->ProjectionMatrix * _renderer->ViewMatrix * modelMat;

		_shader->Bind();
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
		_shader->SetUniformMatrix4fv("u_M", &modelMat[0][0]);

		_renderer->Draw(_ib, _va, _shader);
	}
};