#include "exceptions/ApplicationException.h"

otg::ApplicationException::ApplicationException(const std::string& description) {
	message = "ERROR::APPLICATION::" + description;
}

const char* otg::ApplicationException::what() const noexcept {
	return message.c_str();
}