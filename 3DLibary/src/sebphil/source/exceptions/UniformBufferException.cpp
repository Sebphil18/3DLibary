#include "exceptions/UniformBufferException.h"

namespace glib {

	UniformBufferException::UniformBufferException(const std::string& blockName) {
		msg = "ERROR::UNIFORMBUFFER::Could not locate uniform block '" + blockName + "'!";
	}

	const char* UniformBufferException::what() const {
		return msg.c_str();
	}

}