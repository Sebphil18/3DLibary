#include "globjects/RenderBuffer.h"
#include <memory>
#include "glad/glad.h"

namespace otg {

	RenderBuffer::RenderBuffer() noexcept :
		width(0), height(0), target(GL_RENDERBUFFER), TextureTypes(TextureType::DepthStencilAttachment)
	{
		createRenderbuffer();
	}

	RenderBuffer::RenderBuffer(std::int32_t width, std::int32_t height, TextureType format) noexcept :
		width(width), height(height), target(GL_RENDERBUFFER), TextureTypes(format)
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
		target = other.target;
		type = other.type;

		createRenderbuffer();

		return *this;
	}

	RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept :
		width(std::move(other.width)),
		height(std::move(other.height)),
		target(std::move(other.target)),
		GlObject(std::move(other)),
		TextureTypes(other.type) 	{
	}

	RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		width = std::move(other.width);
		height = std::move(other.height);
		target = std::move(other.target);
		type = std::move(other.type);

		return *this;
	}

	RenderBuffer::~RenderBuffer() noexcept {
		glDeleteRenderbuffers(1, &glHandle);
	}

	void RenderBuffer::setTarget(std::uint32_t target) {
		this->target = target;
	}

	std::uint32_t RenderBuffer::getTarget() const {
		return target;
	}

	void RenderBuffer::createRenderbuffer() {
		glCreateRenderbuffers(1, &glHandle);

		std::uint32_t format = getFormat();

		glNamedRenderbufferStorage(glHandle, format, width, height);
	}

	std::uint32_t RenderBuffer::getFormat() {
		
		switch (type) {

		case TextureType::DepthStencilAttachment:
			return GL_DEPTH24_STENCIL8;
		defualt:
			return GL_RGBA16;
		}

	}

}