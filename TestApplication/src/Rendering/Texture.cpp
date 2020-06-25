#include "Texture.h"

#include <iostream>
#include "stb_image/stb_image.h"

namespace Rendering
{
	Texture::Texture(const std::string& filePath)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mBPP, 4);

		if (!data)
		{
			std::cout << "Could not load texture at " << filePath << std::endl;
			return;
		}


		GLCall(glGenTextures(1, &mRendererId));

		Bind();
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		Unbind();

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		if (mRendererId == 0) return;
		GLCall(glDeleteTextures(1, &mRendererId));
	}

	void Texture::Bind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, mRendererId));
	}

	void Texture::Bind(const unsigned int& slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		Bind();
	}

	void Texture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}