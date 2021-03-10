#include "globjects/Texture.h"
#include "glad/glad.h"
#include <iostream>

namespace otg {

	Texture::Texture() noexcept : 
		img({ nullptr, 0, 0, 0 }), type(TextureType::None)
	{
		createTexture();
	}

	Texture::Texture(std::int32_t width, std::int32_t height, TextureType type) noexcept :
		type(type)
	{
		img.width = width;
		img.height = height;

		createTexture();
		specifyStorage();
	}

	Texture::Texture(const Texture& otherTex) noexcept {

		img = otherTex.img;
		type = otherTex.type;

		createTexture();
		specifyStorage();
	}

	Texture::Texture(const Image& img, TextureType type) noexcept :
		img(img), type(type)
	{
		createTexture();
		specifyStorage();
		specifySubImg();
	}

	Texture& Texture::operator=(const Texture& otherTex) noexcept {

		img = otherTex.img;
		type = otherTex.type;

		createTexture();
		specifyStorage();
		specifySubImg();

		return *this;
	}

	void Texture::createTexture() {

		glCreateTextures(getGlType(type), 1, &glHandle);

		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void Texture::specifyStorage() {

		if (type == TextureType::Albedo)
			glTextureStorage2D(glHandle, 1, GL_SRGB8_ALPHA8, img.width, img.height);
		else
			glTextureStorage2D(glHandle, 1, GL_RGBA16, img.width, img.height);
	}

	void Texture::specifySubImg() {

		if (img.buffer != nullptr)
			glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGBA, GL_UNSIGNED_BYTE, img.buffer);
	}

	Texture::Texture(Texture&& otherTex) noexcept :
		GlObject(std::move(otherTex)),
		img(std::move(otherTex.img)),
		type(std::move(otherTex.type)) {
	}

	Texture& Texture::operator=(Texture&& otherTex) noexcept {

		img = std::move(otherTex.img);
		type = std::move(otherTex.type);

		glHandle = std::move(otherTex.glHandle);
		otherTex.glHandle = 0;

		return *this;
	}

	Texture::~Texture() noexcept {
		glDeleteTextures(1, &glHandle);
	}

	void Texture::generateMipmap() {

		glGenerateTextureMipmap(glHandle);

		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameterf(glHandle, GL_TEXTURE_LOD_BIAS, 0.1);
	}

	TextureType otg::Texture::getType() const {
		return type;
	}

}