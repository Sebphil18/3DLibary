#include "globjects/HDRTexture.h"
#include <iostream>
#include <memory>
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "exceptions/ImageLoadException.h"

namespace otg {

	HDRTexture::HDRTexture(const std::string& filePath) noexcept : 
		filePath(filePath)
	{
		init();
	}

	HDRTexture::HDRTexture(const HDRTexture& other) noexcept : 
		filePath(other.filePath)
	{
		init();
	}

	HDRTexture& HDRTexture::operator=(const HDRTexture& other) noexcept {

		filePath = other.filePath;

		deleteTexture();
		init();

		return *this;
	}

	void HDRTexture::init() {

		tryLoadImage();
		createTexture();
		specifyStorage();
		fillStorage();
	}

	void HDRTexture::tryLoadImage() {

		try {
			loadImage();
		} catch (const ImageLoaderException& exception) {
			std::cout << exception.what() << "\n";
		}
	}

	void HDRTexture::loadImage() {

		img.buffer = stbi_loadf(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		if (!img.buffer)
			throw ImageLoaderException(filePath);
	}

	void HDRTexture::createTexture() {

		glCreateTextures(GL_TEXTURE_2D, 1, &glHandle);

		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	void HDRTexture::specifyStorage() {
		glTextureStorage2D(glHandle, 1, GL_RGB16F, img.width, img.height);
	}

	void HDRTexture::fillStorage() {
		glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGB, GL_FLOAT, img.buffer);
	}

	HDRTexture::HDRTexture(HDRTexture&& other) noexcept : 
		GlObject(std::move(other)),
		filePath(std::move(other.filePath)),
		img(std::move(other.img))
	{
	}

	HDRTexture& HDRTexture::operator=(HDRTexture&& other) noexcept {

		deleteTexture();

		GlObject::operator=(std::move(other));

		filePath = std::move(other.filePath);
		img = std::move(other.img);

		return *this;
	}

	HDRTexture::~HDRTexture() noexcept {
		deleteTexture();
	}

	void HDRTexture::deleteTexture() {
		stbi_image_free(img.buffer);
		glDeleteTextures(1, &glHandle);
	}

	void HDRTexture::bindToUnit(std::uint32_t unit) {
		glBindTextureUnit(unit, glHandle);
	}

}