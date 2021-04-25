#include "globjects/Framebuffer.h"
#include <iostream>
#include "glad/glad.h"
#include "exceptions/FramebufferException.h"

namespace otg {

	float Framebuffer::clearColor[4] = { 0.01, 0, 0.01, 1 };

	Framebuffer::Framebuffer() noexcept :
		target(GL_FRAMEBUFFER)
	{
		createFramebuffer();
	}

	Framebuffer::Framebuffer(const Framebuffer& other) noexcept :
		Framebuffer()
	{
	}

	Framebuffer& Framebuffer::operator=(const Framebuffer& other) noexcept {

		return *this;
	}

	void Framebuffer::createFramebuffer() {
		glCreateFramebuffers(1, &glHandle);
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept :
		GlObject(std::move(other)),
		target(std::move(other.target))
	{
	}

	Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		target = std::move(other.target);

		return *this;
	}

	Framebuffer::~Framebuffer() noexcept {
		glDeleteFramebuffers(1, & glHandle);
	}

	void Framebuffer::attachTexture(const Texture& texture, std::uint32_t attachmnetSlot) {

		TextureType type = texture.getType();

		switch (type) {

		case TextureType::DepthStencilAttachment: 
			glNamedFramebufferTexture(glHandle, GL_DEPTH_STENCIL_ATTACHMENT, texture.getGlHandle(), 0);
			break;

		default:
			std::uint32_t attachmentSlot = GL_COLOR_ATTACHMENT0 + attachmnetSlot;
			glNamedFramebufferTexture(glHandle, attachmentSlot, texture.getGlHandle(), 0);
			break;
		}
	}

	void Framebuffer::attachTexture(const MultisampleTexture& texture, std::uint32_t attachmentSlot) {

		glNamedFramebufferTexture(glHandle, GL_COLOR_ATTACHMENT0 + attachmentSlot, texture.getGlHandle(), 0);
	}

	void Framebuffer::attachTextureLayer(std::uint32_t textureHandle, std::uint32_t mipmapLevel, std::uint32_t layer, std::uint32_t attachmentSlot) {

		glNamedFramebufferTextureLayer(glHandle, GL_COLOR_ATTACHMENT0 + attachmentSlot, textureHandle, mipmapLevel, layer);
	}

	void Framebuffer::attachRenderBuffer(const RenderBuffer& renderbuffer, std::uint32_t attachmentSlot) {

		TextureType type = renderbuffer.getType();

		switch (type) {

		case TextureType::DepthStencilAttachment:
			glNamedFramebufferRenderbuffer(glHandle, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.getGlHandle());
			break;

		default:
			std::uint32_t attachment = GL_COLOR_ATTACHMENT0 + attachmentSlot;
			glNamedFramebufferRenderbuffer(glHandle, attachment, GL_RENDERBUFFER, renderbuffer.getGlHandle());
			break;
		}
	}

	void Framebuffer::attachRenderBuffer(const MultisampleRenderBuffer& renderbuffer, std::uint32_t attachmentSlot) {

		TextureType type = renderbuffer.getType();

		switch (type) {

		case TextureType::DepthStencilAttachment:
			glNamedFramebufferRenderbuffer(glHandle, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.getGlHandle());
			break;

		default:
			std::uint32_t attachment = GL_COLOR_ATTACHMENT0 + attachmentSlot;
			glNamedFramebufferRenderbuffer(glHandle, attachment, GL_RENDERBUFFER, renderbuffer.getGlHandle());
			break;
		}
	}

	void Framebuffer::validate() {

		try {
			tryValidation();
		} catch (const FramebufferException& exception) {
			std::cout << exception.what() << "\n";
		}
	}

	void Framebuffer::tryValidation() {

		std::uint32_t status = glCheckNamedFramebufferStatus(glHandle, target);

		if (status != GL_FRAMEBUFFER_COMPLETE)
			throw FramebufferException();
	}

	void Framebuffer::bind() const {
		glBindFramebuffer(target, glHandle);
	}

	void Framebuffer::unbind() const {
		glBindFramebuffer(target, 0);
	}

	void Framebuffer::clear() {

		glClearNamedFramebufferfv(glHandle, GL_COLOR, 0, clearColor);
		glClearNamedFramebufferfi(glHandle, GL_DEPTH_STENCIL, 0, 1, 0);
	}

	void Framebuffer::copyColorTo(std::int32_t width, std::int32_t height, otg::Framebuffer& other) {
		glBlitNamedFramebuffer(glHandle, other.getGlHandle(), 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	std::uint32_t Framebuffer::setTarget(std::uint32_t target) {
		return target;
	}
}