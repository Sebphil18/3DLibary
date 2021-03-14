#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"

namespace otg {

	class RenderBuffer : public GlObject, public TextureTypes {

	public:
		RenderBuffer() noexcept;
		RenderBuffer(std::int32_t width, std::int32_t height, TextureType format) noexcept;
		RenderBuffer(const RenderBuffer& other) noexcept;
		RenderBuffer(RenderBuffer&& other) noexcept;

		RenderBuffer& operator=(const RenderBuffer& other) noexcept;
		RenderBuffer& operator=(RenderBuffer && other) noexcept;

		~RenderBuffer() noexcept;

		void setTarget(std::uint32_t target);

		std::uint32_t getTarget() const;

	private:
		std::int32_t width, height;
		std::uint32_t target;

		void createRenderbuffer();
		std::uint32_t getFormat();

	};

}
