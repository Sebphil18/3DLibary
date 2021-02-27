#include "globjects/GlObject.h"

namespace otg {

	otg::GlObject::GlObject():
		glHandle(0)
	{
	}

	otg::GlObject::GlObject(std::uint32_t glHandle):
		glHandle(glHandle)
	{
	}

	std::uint32_t GlObject::getGlHandle() const noexcept {
		return glHandle;
	}
}