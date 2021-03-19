#pragma once
#include "globjects/GlObject.h"
#include "globjects/Texture.h"
#include "globjects/MultisampleTexture.h"
#include "globjects/RenderBuffer.h"
#include "globjects/MultisampleRenderBuffer.h"

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
		void attachTexture(const MultisampleTexture& texture);

		void attachRenderBuffer(const RenderBuffer& renderbuffer);
		void attachRenderBuffer(const MultisampleRenderBuffer& renderbuffer);

		void validate();

		void bind() const;
		void unbind() const;

		void clear();
		void copyColorTo(std::int32_t width, std::int32_t height, otg::Framebuffer& other);

		std::uint32_t setTarget(std::uint32_t target);

	private:
		std::uint32_t target;
		std::uint32_t colorAttachement;

		static float clearColor[4];

		void createFramebuffer();

		void tryValidation();

	};

}
