#include "globjects/GlObject.h"
#include <memory>

namespace otg {

	otg::GlObject::GlObject() noexcept :
		glHandle(0)
	{
	}

	otg::GlObject::GlObject(std::uint32_t glHandle) noexcept :
		glHandle(glHandle)
	{
	}

	otg::GlObject::GlObject(GlObject&& otherObj) noexcept :
		glHandle(std::move(otherObj.glHandle))
	{
		otherObj.glHandle = 0;
	}

	std::uint32_t GlObject::getGlHandle() const noexcept {
		return glHandle;
	}
}