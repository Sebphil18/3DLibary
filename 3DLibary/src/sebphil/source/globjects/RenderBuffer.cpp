#include "globjects/RenderBuffer.h"
#include <memory>
#include "glad/glad.h"

namespace glib {

	RenderBuffer::RenderBuffer() noexcept :
		width(1), height(1), TextureTypes(TextureType::DepthStencilAttachment)
	{
		createRenderbuffer();
	}

	RenderBuffer::RenderBuffer(std::int32_t width, std::int32_t height, TextureType type) noexcept :
		width(width), height(height), TextureTypes(type)
	{
		createRenderbuffer();
	}

	RenderBuffer::RenderBuffer(const RenderBuffer& other) noexcept : 
		RenderBuffer(other.width, other.height, other.type)
	{
	}

	RenderBuffer& RenderBuffer::operator=(const RenderBuffer& other) noexcept {

		width = other.width;
		height = other.height;
		type = other.type;

		specifyStorage();

		return *this;
	}

	void RenderBuffer::createRenderbuffer() {

		glCreateRenderbuffers(1, &glHandle);

		specifyStorage();
	}

	void RenderBuffer::setSize(std::int32_t width, std::int32_t height) {

		this->width = width;
		this->height = height;

		specifyStorage();
	}

	void RenderBuffer::specifyStorage() {

		std::uint32_t format = getGlFormat(type);
		glNamedRenderbufferStorage(glHandle, format, width, height);
	}

	RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept :
		width(std::move(other.width)),
		height(std::move(other.height)),
		GlObject(std::move(other)),
		TextureTypes(other.type) 	
	{
	}

	RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		width = std::move(other.width);
		height = std::move(other.height);
		type = std::move(other.type);

		return *this;
	}

	RenderBuffer::~RenderBuffer() noexcept {
		glDeleteRenderbuffers(1, &glHandle);
	}

}