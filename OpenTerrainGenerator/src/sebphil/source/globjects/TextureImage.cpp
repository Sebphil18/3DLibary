#include "globjects/TextureImage.h"
#include "stb/stb_image.h"
#include "glad/glad.h"

namespace otg {

	TextureImage::TextureImage() noexcept :
		img({ nullptr, 0, 0, 0 }), type(TextureType::None) {

		createTexture();
	}

	TextureImage::TextureImage(const std::string& filePath, TextureType type) noexcept :
		filePath(filePath), type(type) {

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		initialize();
	}

	TextureImage::TextureImage(const TextureImage& otherTex) noexcept :
		img(otherTex.img), type(otherTex.type), filePath(otherTex.filePath) {

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		initialize();
	}

	TextureImage& TextureImage::operator=(const TextureImage& otherTex) noexcept {

		img = otherTex.img;
		type = otherTex.type;
		filePath = otherTex.filePath;

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		initialize();

		return *this;
	}

	void TextureImage::initialize() {

		createTexture();
		setTextureData();
		setTextureParams();
		generateMipmap();
	}

	void otg::TextureImage::setImage(const std::string& filePath, TextureType type) {

		this->filePath = filePath;
		this->type = type;

		img.buffer = stbi_load(filePath.c_str(), &img.width, &img.height, &img.channels, 0);

		setTextureData();
		setTextureParams();
		generateMipmap();
	}

	void TextureImage::createTexture() {
		glCreateTextures(GL_TEXTURE_2D, 1, &glHandle);
	}

	void TextureImage::setTextureData() {

		if (type == TextureType::Albedo)
			glTextureStorage2D(glHandle, 1, GL_SRGB8_ALPHA8, img.width, img.height);
		else
			glTextureStorage2D(glHandle, 1, GL_RGBA16, img.width, img.height);

		glTextureSubImage2D(glHandle, 0, 0, 0, img.width, img.height, GL_RGBA, GL_UNSIGNED_BYTE, img.buffer);
	}

	void TextureImage::setTextureParams() {

		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTextureParameterf(glHandle, GL_TEXTURE_LOD_BIAS, 0.1f);
	}

	void TextureImage::generateMipmap() {

		glGenerateTextureMipmap(glHandle);
	}

	TextureImage::TextureImage(TextureImage&& otherTex) noexcept :
		img(std::move(otherTex.img)),
		type(std::move(otherTex.type)),
		filePath(std::move(otherTex.filePath)),
		GlObject(std::move(otherTex.glHandle)) {
	}

	TextureImage& TextureImage::operator=(TextureImage&& otherTex) noexcept {

		img = std::move(otherTex.img);
		type = std::move(otherTex.type);
		filePath = std::move(otherTex.filePath);
		glHandle = std::move(otherTex.glHandle);

		return *this;
	}

	TextureImage::~TextureImage() noexcept {

		stbi_image_free(img.buffer);
		glDeleteTextures(1, &glHandle);
	}

	TextureType TextureImage::getType() const {
		return type;
	}

	unsigned char* TextureImage::getBufferPtr() const {
		return img.buffer;
	}

}