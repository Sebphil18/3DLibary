#include "exceptions/FileInputException.h"

namespace glib {

	glib::FileInputException::FileInputException(const std::string& filePath) {
		message = "ERROR::FILE::INPUT::Could not open file '" + filePath + "'!";
	}

	const char* glib::FileInputException::what() const {
		return message.c_str();
	}

}