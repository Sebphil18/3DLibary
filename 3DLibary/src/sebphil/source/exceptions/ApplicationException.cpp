#include "exceptions/ApplicationException.h"

glib::ApplicationException::ApplicationException(const std::string& description) {
	message = "ERROR::APPLICATION::" + description;
}

const char* glib::ApplicationException::what() const noexcept {
	return message.c_str();
}