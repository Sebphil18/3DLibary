#include "exceptions/FramebufferException.h"

namespace glib {

	FramebufferException::FramebufferException() {
		message = "ERROR::FRAMEBUFFER::Framebuffer is incomplete!";
	}
	const char* FramebufferException::what() const noexcept {
		return message.c_str();
	}
}