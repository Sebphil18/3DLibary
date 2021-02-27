#pragma once
#include <cstdint>

namespace otg {

	class GlObject {

	public:
		GlObject();
		GlObject(std::uint32_t glHandle);
		virtual std::uint32_t getGlHandle() const noexcept;

	protected:
		std::uint32_t glHandle;

	};

}
