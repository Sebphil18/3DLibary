#include "globjects/TextureImage.h"
#include <iostream>
#include "stb/stb_image.h"
#include "glad/glad.h"
#include "exceptions/ImageLoadException.h"

namespace otg {

	TextureImage::TextureImage() noexcept :
		Texture() 
	{
	}

	TextureImage::TextureImage(const std::string& filePath, TextureType type) noexcept :
		filePath(filePath)
	{
		this->type = type;
		loadTexture();
	}

	TextureImage::TextureImage(const TextureImage& otherTex) noexcept :
		TextureImage(otherTex.filePath, otherTex.type)
	{
	}

	TextureImage& TextureImage::operator=(const TextureImage& otherTex) noexcept {

		Texture::operator=(otherTex);

		filePath = otherTex.filePath;

		// TODO: This might lead to a bug because OpenGl will hold a data pointer to the original img.buffer which could be deleted before this object
		tryLoadImg();

		return *this;
	}

	void otg::TextureImage::setImage(const std::string& filePath, TextureType type) {

		this->filePath = filePath;
		this->type = type;

		loadTexture();
	}

	void TextureImage::loadTexture() {

		tryLoadImg();
		specifyStorage();
		specifySubImg();
		generateMipmap();
	}

	void otg::TextureImage::tryLoadImg() {

		try {
			loadImg();
		} catch (const ImageLoaderException& exception) {
			std::cout << exception.what() << "\n";
		}
	}

	void otg::TextureImage::loadImg() {

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		if (!img.buffer)
			throw ImageLoaderException(filePath);
	}

	void otg::TextureImage::specifyStorage() {
		glTextureStorage2D(glHandle, 1, GL_SRGB8_ALPHA8, img.width, img.height);
	}

	TextureImage::TextureImage(TextureImage&& otherTex) noexcept :
		Texture(std::move(otherTex)),
		filePath(std::move(otherTex.filePath))
	{
	}

	TextureImage& TextureImage::operator=(TextureImage&& otherTex) noexcept {

		Texture::operator=(std::move(otherTex));

		filePath = std::move(otherTex.filePath);

		return *this;
	}

	TextureImage::~TextureImage() noexcept {
		stbi_image_free(img.buffer);
	}

	TextureType TextureImage::getType() const {
		return type;
	}

	unsigned char* TextureImage::getBufferPtr() const {
		return img.buffer;
	}

}