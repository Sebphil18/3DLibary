#pragma once
#include "globjects/GlObject.h"
#include "globjects/Texture.h"
#include "globjects/RenderBuffer.h"

namespace otg {

	class Framebuffer : public GlObject {

	public:
		Framebuffer() noexcept;
		Framebuffer(const Framebuffer& other) noexcept;
		Framebuffer(Framebuffer&& other) noexcept;

		Framebuffer& operator=(const Framebuffer& other) noexcept;
		Framebuffer& operator=(Framebuffer&& other) noexcept;

		~Framebuffer() noexcept;

		void attachTexture(const Texture& texture);
		void attachRenderBuffer(const RenderBuffer& renderbuffer);

		void validate();

		void bind() const;
		void unbind() const;

		std::uint32_t setTarget(std::uint32_t target);

	private:
		std::uint32_t target;
		std::uint32_t colorAttachement;

		void createFramebuffer();

		void setTextureAttachment(const Texture& texture, std::uint32_t& attachmentItr, std::uint32_t attachment);
		void setRenderbufferAttachment(const RenderBuffer& renderbuffer, std::uint32_t& attachmentItr, std::uint32_t attachment);

		void tryValidation();

	};

}
