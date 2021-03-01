#include "globjects/TextureImage2D.h"
#include "glad/glad.h"

namespace otg {

	TextureImage2D::TextureImage2D(const Image& image, std::uint32_t textureUnit) :
		image(image)
	{
		createTexture(textureUnit);
		setStorage();
		setParameter();
	}

	void TextureImage2D::createTexture(std::uint32_t textureUnit) {
		glCreateTextures(GL_TEXTURE_2D, 1, &glHandle);
		glBindTextureUnit(textureUnit, glHandle);
		glGenerateTextureMipmap(glHandle);
	}

	void TextureImage2D::setParameter() {
		glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void TextureImage2D::setImage(const Image& image) {

		this->image = image;
		setStorage();
	}

	void TextureImage2D::setStorage() {
		glTextureStorage2D(glHandle, 1, GL_SRGB8_ALPHA8, image.width, image.height);
		glTextureSubImage2D(glHandle, 0, 0, 0, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.buffer);
	}


}