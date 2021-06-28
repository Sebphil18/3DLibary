#include "globjects/Texture.h"
#include "glad/glad.h"
#include <iostream>

namespace glib {

	Texture::Texture() noexcept : 
		img({ nullptr, 1, 1, 0 }), TextureTypes()
	{
		createTexture();
	}

	glib::Texture::Texture(TextureType type) noexcept :
		img({ nullptr, 1, 1, 0 }), TextureTypes(type)
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

		deleteTexture();
		initTexture();
		specifySubImg();

		return *this;
	}
	
	void Texture::setSize(std::int32_t width, std::int32_t height) {

		img.width = width;
		img.height = height;

		// a new texture has to be created, because DSA only supports immutable textures
		deleteTexture();
		initTexture();
		specifySubImg();
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

	void Texture::specifyStorage(std::uint32_t mipmapLevels) {
		glTextureStorage2D(glHandle, mipmapLevels, getGlFormat(type), img.width, img.height);
	}

	void Texture::specifySubImg() {

		if (img.buffer != nullptr) {

			if(img.channels == 3)
				glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGB, getGlDataType(type), img.buffer);
			else
				glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGBA, getGlDataType(type), img.buffer);
		}
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
		deleteTexture();
	}

	void Texture::deleteTexture() {
		glDeleteTextures(1, &glHandle);
	}

	void Texture::generateMipmap() {

		glGenerateTextureMipmap(glHandle);

		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameterf(glHandle, GL_TEXTURE_LOD_BIAS, 0);
	}

	void Texture::bindToUnit(std::uint32_t unit) {
		glBindTextureUnit(unit, glHandle);
	}

	std::int32_t Texture::getWidth() const {
		return img.width;
	}

	std::int32_t Texture::getHeight() const {
		return img.height;
	}

}