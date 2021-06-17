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

		void attachTexture(const Texture& texture, std::uint32_t attachmentSlot = 0);
		void attachTexture(const MultisampleTexture& texture, std::uint32_t attachmentSlot = 0);
		// TODO: too many arguments
		void attachTextureLayer(std::uint32_t textureHandle, std::uint32_t mipmapLevel, std::uint32_t layer, std::uint32_t attachmentSlot = 0);

		void attachRenderBuffer(const RenderBuffer& renderbuffer, std::uint32_t attachmentSlot = 0);
		void attachRenderBuffer(const MultisampleRenderBuffer& renderbuffer, std::uint32_t attachmentSlot = 0);

		void validate();

		void bind() const;
		void unbind() const;

		void clear();
		void copyTo(std::int32_t width, std::int32_t height, otg::Framebuffer& other);
		void copyTo(std::int32_t width, std::int32_t height, std::uint32_t other);

		std::uint32_t setTarget(std::uint32_t target);

	private:
		std::uint32_t target;

		static float clearColor[4];

		void createFramebuffer();

		void tryValidation();

	};

}
