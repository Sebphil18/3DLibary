#include "globjects/GlObject.h"
#include <memory>

namespace glib {

	GlObject::GlObject() noexcept :
		glHandle(0)
	{
	}

	GlObject::GlObject(std::uint32_t glHandle) noexcept :
		glHandle(glHandle)
	{
	}

	GlObject::GlObject(GlObject&& otherObj) noexcept :
		glHandle(std::move(otherObj.glHandle))
	{
		otherObj.glHandle = 0;
	}

	GlObject& GlObject::operator=(GlObject&& otherObj) noexcept {

		glHandle = otherObj.glHandle;
		otherObj.glHandle = 0;

		return *this;
	}

	std::uint32_t GlObject::getGlHandle() const noexcept {
		return glHandle;
	}
}