#include "GameObject.h"

namespace Systems
{
	GameObject::GameObject(const Rendering::Renderer* renderer, Rendering::Mesh* mesh,
		Rendering::Texture* texture, unsigned int textureSlot, Rendering::Shader* shader)
			: mRenderer(renderer), mMesh(mesh), mTexture(texture), _shader(shader),
			mTextureSlot(textureSlot)
	{
		if (mMesh == nullptr || mTexture == nullptr || _shader == nullptr)
		{
			std::cout << "One of assets is null. Cannot initialize!\n";
			return;
		}
	}

	GameObject::~GameObject()
	{
		if (mMesh != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(mMesh));
		if (mTexture != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(mTexture));
		if (_shader != nullptr) Resources::AssetDatabase::PutBack(ToAssetReference(_shader));
	}

	void GameObject::Draw() // to renderer component
	{
		glm::mat4 m = Transform.GetMatrix();
		glm::mat4 mvp = mRenderer->ProjectionMatrix * mRenderer->ViewMatrix * m;

		_shader->Bind();
		mTexture->Bind(mTextureSlot);
		_shader->SetUniform1i("u_Texture", mTextureSlot);
		_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		_shader->SetUniform3f("u_LightDirection", 1.0f, 1.0f, 0.0f);
		_shader->SetUniformMatrix4fv("u_Mvp", &mvp[0][0]);
		_shader->SetUniformMatrix4fv("u_M", &m[0][0]);

		mRenderer->Draw(*mMesh->GetIB(), *mMesh->GetVA(), _shader);
	}
}