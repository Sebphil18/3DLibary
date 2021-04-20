#include "globjects/TextureImage.h"
#include <iostream>
#include "glad/glad.h"
#include "stb/stb_image.h"
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

		tryLoadImg();

		return *this;
	}

	void TextureImage::setImage(const std::string& filePath, TextureType type) {

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

	void TextureImage::tryLoadImg() {

		try {
			loadImg();
		} catch (const ImageLoaderException& exception) {
			std::cout << exception.what() << "\n";
		}
	}

	void TextureImage::loadImg() {

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		if (!img.buffer)
			throw ImageLoaderException(filePath);
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