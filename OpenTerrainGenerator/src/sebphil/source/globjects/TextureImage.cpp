#include "globjects/TextureImage.h"
#include "stb/stb_image.h"
#include "glad/glad.h"

namespace otg {

	TextureImage::TextureImage() noexcept :
		Texture() 
	{
	}

	TextureImage::TextureImage(const std::string& filePath, TextureType type) noexcept :
		filePath(filePath)
	{
		this->type = type;
		loadImg();
	}

	TextureImage::TextureImage(const TextureImage& otherTex) noexcept :
		TextureImage(otherTex.filePath, otherTex.type)
	{
	}

	TextureImage& TextureImage::operator=(const TextureImage& otherTex) noexcept {

		Texture::operator=(otherTex);

		filePath = otherTex.filePath;

		// TODO: loadImg
		// TODO: This might lead to a bug because OpenGl will hold a data pointer to the original img.buffer which could be deleted before this object
		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		return *this;
	}

	void otg::TextureImage::setImage(const std::string& filePath, TextureType type) {

		this->filePath = filePath;
		this->type = type;

		loadImg();
	}

	void TextureImage::loadImg() {

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);
		specifyStorage();
		specifySubImg();
		generateMipmap();
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