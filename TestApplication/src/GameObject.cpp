#include "GameObject.h"

GameObject::GameObject(const Rendering::Renderer* renderer, Rendering::Mesh* mesh,
	Rendering::Texture* texture, unsigned int textureSlot, Rendering::Shader* shader)
		: _renderer(renderer), _mesh(mesh), _texture(texture), _shader(shader),
		_textureSlot(textureSlot)
{
	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
	{
		std::cout << "One of assets is null. Cannot initialize!\n";
		return;
	}
}

GameObject::~GameObject()
{
	if (_mesh != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_mesh));
	if (_texture != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_texture));
	if (_shader != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_shader));
}

void GameObject::Draw() // to renderer component
{
	glm::mat4 modelMat = Transform.GetMatrix();

	glm::mat4 mvp = _renderer->ProjectionMatrix * _renderer->ViewMatrix * modelMat;

	_shader->Bind();
	_texture->Bind(_textureSlot);
	_shader->SetUniform1i("u_Texture", _textureSlot);
	_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
	_shader->SetUniformMatrix4fv("u_M", &modelMat[0][0]);

	_renderer->Draw(*_mesh->GetIB(), *_mesh->GetVA(), _shader);
}