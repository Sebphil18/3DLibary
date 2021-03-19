#include "globjects/Texture.h"
#include "glad/glad.h"
#include <iostream>

namespace otg {

	Texture::Texture() noexcept : 
		img({ nullptr, 0, 0, 0 }), TextureTypes()
	{
		createTexture();
	}

	otg::Texture::Texture(TextureType type) noexcept :
		img({ nullptr, 0, 0, 0 }), TextureTypes(type)
	{
		createTexture();
	}

	Texture::Texture(std::int32_t width, std::int32_t height, TextureType type) noexcept :
		TextureTypes(type)
	{
		img.width = width;
		img.height = height;

		initTexture();
	}

	Texture::Texture(const Texture& other) noexcept :
		img(other.img), TextureTypes(other.type)
	{
		initTexture();
	}

	Texture::Texture(const Image& img, TextureType type) noexcept :
		img(img), TextureTypes(type)
	{
		initTexture();
		specifySubImg();
	}

	Texture& Texture::operator=(const Texture& other) noexcept {

		img = other.img;
		type = other.type;

		initTexture();
		specifySubImg();

		return *this;
	}
	
	void Texture::initTexture() {
		createTexture();
		specifyStorage();
	}

	void Texture::createTexture() {

		glCreateTextures(getGlType(type), 1, &glHandle);

		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void Texture::specifyStorage() {
		glTextureStorage2D(glHandle, 1, getGlFormat(type), img.width, img.height);
	}

	void Texture::specifySubImg() {

		if (img.buffer != nullptr)
			glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGBA, getGlDataType(type), img.buffer);
	}

	Texture::Texture(Texture&& other) noexcept :
		GlObject(std::move(other)),
		TextureTypes(other.type),
		img(std::move(other.img))
	{
	}

	Texture& Texture::operator=(Texture&& other) noexcept {

		GlObject::operator=(std::move(other));

		img = std::move(other.img);
		type = std::move(other.type);

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

}