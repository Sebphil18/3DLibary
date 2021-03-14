#include "exceptions/FramebufferException.h"

namespace otg {

	FramebufferException::FramebufferException() {
		message = "ERROR::FRAMEBUFFER::Framebbufer is incomplete!";
	}
	const char* FramebufferException::what() const noexcept {
		return message.c_str();
	}
}