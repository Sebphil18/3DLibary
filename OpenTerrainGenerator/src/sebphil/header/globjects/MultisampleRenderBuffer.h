#pragma once

#include "globjects/GlObject.h"
#include "globjects/TextureType.h"

namespace otg {

	class MultisampleRenderBuffer : public GlObject, public TextureTypes {

	public:
		MultisampleRenderBuffer() noexcept;
		MultisampleRenderBuffer(std::int32_t width, std::int32_t height) noexcept;
		MultisampleRenderBuffer(std::int32_t width, std::int32_t height, std::uint32_t samples) noexcept;
		MultisampleRenderBuffer(const MultisampleRenderBuffer& other) noexcept;
		MultisampleRenderBuffer(MultisampleRenderBuffer&& other) noexcept;

		MultisampleRenderBuffer& operator=(const MultisampleRenderBuffer& other) noexcept;
		MultisampleRenderBuffer& operator=(MultisampleRenderBuffer&& other) noexcept;

		~MultisampleRenderBuffer() noexcept;

		void setSize(std::int32_t width, std::int32_t height);

	private:
		std::int32_t width, height;
		std::uint32_t samples;

		void createRenderBuffer();
		void specifyStorage();

	};

}