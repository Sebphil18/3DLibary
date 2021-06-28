#pragma once
#include <cstdint>

namespace glib {

	class GlObject {

	public:
		GlObject() noexcept;
		GlObject(std::uint32_t glHandle) noexcept;
		GlObject(const GlObject& otherobj) = delete;
		GlObject(GlObject&& otherObj) noexcept;

		GlObject& operator=(const GlObject& otherObj) = delete;
		GlObject& operator=(GlObject&& otherObj) noexcept;

		std::uint32_t getGlHandle() const noexcept;

	protected:
		std::uint32_t glHandle;

	};

}
