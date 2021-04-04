#include "globjects/MultisampleTexture.h"
#include "glad/glad.h"
#include <iostream>

namespace otg {

	MultisampleTexture::MultisampleTexture() noexcept :
		samples(4), img({nullptr, 1, 1, 4}), TextureTypes(TextureType::Multisample)
	{
		createTexture();
	}

	MultisampleTexture::MultisampleTexture(std::int32_t width, std::int32_t height) noexcept : 
		samples(4), TextureTypes(TextureType::Multisample)
	{
		img.width = width;
		img.height = height;

		initTexture();
	}

	MultisampleTexture::MultisampleTexture(std::int32_t width, std::int32_t height, std::uint32_t samples) noexcept :
		samples(samples), TextureTypes(TextureType::Multisample)
	{
		img.width = width;
		img.height = height;

		initTexture();
	}

	MultisampleTexture::MultisampleTexture(const MultisampleTexture& other) noexcept :
		img(other.img), samples(other.samples), TextureTypes(other.type)
	{
		initTexture();
	}

	MultisampleTexture& MultisampleTexture::operator=(const MultisampleTexture& other) noexcept {

		img = other.img;
		type = other.type;
		samples = other.samples;

		delteTexture();
		initTexture();

		return *this;
	}

	void MultisampleTexture::setSize(std::int32_t width, std::int32_t height) {

		img.width = width;
		img.height = height;

		delteTexture();
		initTexture();
	}

	void MultisampleTexture::initTexture() {
		createTexture();
		specifyStorage();
	}

	void MultisampleTexture::createTexture() {
		glCreateTextures(getGlType(type), 1, &glHandle);
	}

	void MultisampleTexture::specifyStorage() {
		glTextureStorage2DMultisample(glHandle, samples, getGlFormat(type), img.width, img.height, true);
	}

	MultisampleTexture::MultisampleTexture(MultisampleTexture&& other) noexcept : 
		GlObject(std::move(other)),
		TextureTypes(other.type),
		img(std::move(other.img)),
		samples(std::move(other.samples))
	{
	}

	MultisampleTexture& MultisampleTexture::operator=(MultisampleTexture&& other) noexcept {

		GlObject::operator=(std::move(other));

		img = std::move(other.img);
		samples = std::move(other.samples);
		type = std::move(other.type);

		return *this;
	}

	MultisampleTexture::~MultisampleTexture() noexcept {
		delteTexture();
	}

	void MultisampleTexture::delteTexture() {
		glDeleteTextures(1, &glHandle);
	}

}