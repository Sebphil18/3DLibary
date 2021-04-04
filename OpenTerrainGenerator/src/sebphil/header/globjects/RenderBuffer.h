#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"

namespace otg {

	class RenderBuffer : public GlObject, public TextureTypes {

	public:
		RenderBuffer() noexcept;
		RenderBuffer(std::int32_t width, std::int32_t height, TextureType type) noexcept;
		RenderBuffer(const RenderBuffer& other) noexcept;
		RenderBuffer(RenderBuffer&& other) noexcept;

		RenderBuffer& operator=(const RenderBuffer& other) noexcept;
		RenderBuffer& operator=(RenderBuffer && other) noexcept;

		~RenderBuffer() noexcept;

		void setSize(std::int32_t width, std::int32_t height);

	private:
		std::int32_t width, height;

		void createRenderbuffer();
		void specifyStorage();

	};

}
