#include "globjects/MultisampleRenderBuffer.h"
#include <iostream>
#include "glad/glad.h"

namespace otg {

	MultisampleRenderBuffer::MultisampleRenderBuffer() noexcept : 
		width(0), height(0), samples(4), TextureTypes(TextureType::DepthStencilAttachment)
	{
		glCreateRenderbuffers(1, &glHandle);
	}

	MultisampleRenderBuffer::MultisampleRenderBuffer(std::int32_t width, std::int32_t height) noexcept : 
		width(width), height(height), samples(4), TextureTypes(TextureType::DepthStencilAttachment)
	{
		createRenderBuffer();
	}

	MultisampleRenderBuffer::MultisampleRenderBuffer(std::int32_t width, std::int32_t height, std::uint32_t samples) noexcept : 
		width(width), height(height), samples(samples), TextureTypes(TextureType::DepthStencilAttachment)
	{
		createRenderBuffer();
	}

	MultisampleRenderBuffer::MultisampleRenderBuffer(const MultisampleRenderBuffer& other) noexcept :
		width(other.width), height(other.height), samples(other.samples), TextureTypes(other.type)
	{
		createRenderBuffer();
	}

	MultisampleRenderBuffer& MultisampleRenderBuffer::operator=(const MultisampleRenderBuffer& other) noexcept {

		width = other.width;
		height = other.height;
		samples = other.samples;
		type = other.type;

		specifyStorage();

		return *this;
	}

	void MultisampleRenderBuffer::createRenderBuffer() {

		glCreateRenderbuffers(1, &glHandle);

		specifyStorage();
	}

	void MultisampleRenderBuffer::setSize(std::int32_t width, std::int32_t height) {

		this->width = width;
		this->height = height;

		specifyStorage();
	}

	void MultisampleRenderBuffer::specifyStorage() {

		std::uint32_t format = getGlFormat(type);
		glNamedRenderbufferStorageMultisample(glHandle, samples, format, width, height);
	}

	MultisampleRenderBuffer::MultisampleRenderBuffer(MultisampleRenderBuffer&& other) noexcept : 
		width(std::move(other.width)), 
		height(std::move(other.height)), 
		samples(std::move(other.samples)),
		TextureTypes(std::move(other.type)),
		GlObject(std::move(other))
	{
	}

	MultisampleRenderBuffer& MultisampleRenderBuffer::operator=(MultisampleRenderBuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		width = std::move(other.width);
		height = std::move(other.height);
		samples = std::move(other.samples);
		type = std::move(other.type);

		return *this;
	}

	MultisampleRenderBuffer::~MultisampleRenderBuffer() noexcept {
		glDeleteRenderbuffers(1, &glHandle);
	}

}