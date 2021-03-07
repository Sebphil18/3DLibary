#pragma once
#include <cstdint>

namespace otg {

	class GlObject {

	public:
		GlObject() noexcept;
		GlObject(std::uint32_t glHandle) noexcept;
		GlObject(GlObject&& otherObj) noexcept;
		virtual std::uint32_t getGlHandle() const noexcept;

	protected:
		std::uint32_t glHandle;

	};

}
