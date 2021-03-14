#include "globjects/Framebuffer.h"
#include <iostream>
#include "glad/glad.h"
#include "exceptions/FramebufferException.h"

namespace otg {

	Framebuffer::Framebuffer() noexcept :
		colorAttachement(0), target(GL_FRAMEBUFFER)
	{
		createFramebuffer();
	}

	Framebuffer::Framebuffer(const Framebuffer& other) noexcept :
		Framebuffer()
	{
	}

	Framebuffer& Framebuffer::operator=(const Framebuffer& other) noexcept {

		createFramebuffer();

		colorAttachement = 0;
		target = GL_FRAMEBUFFER;

		return *this;
	}

	void Framebuffer::createFramebuffer() {
		glCreateFramebuffers(1, &glHandle);
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept :
		GlObject(std::move(other)),
		colorAttachement(std::move(other.colorAttachement)),
		target(std::move(other.target))
	{
	}

	Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		colorAttachement = std::move(other.colorAttachement);
		target = std::move(other.target);

		return *this;
	}

	Framebuffer::~Framebuffer() noexcept {
		glDeleteFramebuffers(1, & glHandle);
	}

	void Framebuffer::attachTexture(const Texture& texture) {

		TextureType type = texture.getType();
		std::uint32_t itr = 0;

		switch (type) {

		case TextureType::DepthStencilAttachment: 
			setTextureAttachment(texture, itr, GL_DEPTH_STENCIL_ATTACHMENT);
			break;

		default:
			setTextureAttachment(texture, colorAttachement, GL_COLOR_ATTACHMENT0);
			break;
		}
	}

	void Framebuffer::setTextureAttachment(const Texture& texture, std::uint32_t& attachmentItr, std::uint32_t attachment) {

		std::uint32_t handle = texture.getGlHandle();

		std::uint32_t attachmentSlot = attachment + attachmentItr;
		attachmentItr++;

		glNamedFramebufferTexture(glHandle, attachmentSlot, handle, 0);
	}

	void Framebuffer::attachRenderBuffer(const RenderBuffer& renderbuffer) {

		TextureType type = renderbuffer.getType();

		std::uint32_t itr = 0;

		switch (type) {

		case TextureType::DepthStencilAttachment:
			setRenderbufferAttachment(renderbuffer, itr, GL_DEPTH_STENCIL_ATTACHMENT);
			break;

		default:
			setRenderbufferAttachment(renderbuffer, colorAttachement, GL_COLOR_ATTACHMENT0);
			break;
		}
	}

	// TODO: not quite nice
	void Framebuffer::setRenderbufferAttachment(const RenderBuffer& renderbuffer, std::uint32_t& attachmentItr, std::uint32_t attachment) {

		std::uint32_t target = renderbuffer.getTarget();
		std::uint32_t handle = renderbuffer.getGlHandle();

		std::uint32_t attachmentSlot = attachment;

		if (renderbuffer.getType() != TextureType::DepthStencilAttachment) {
			attachmentSlot = attachment + attachmentItr;
			attachmentItr++;
		}

		glNamedFramebufferRenderbuffer(glHandle, attachmentSlot, target, handle);
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

	std::uint32_t Framebuffer::setTarget(std::uint32_t target) {
		return target;
	}

}