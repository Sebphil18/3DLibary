#include "exceptions/FileInputException.h"

namespace otg {

	otg::FileInputException::FileInputException(const std::string& filePath) {
		message = "ERROR::FILE::INPUT::Could not open file '" + filePath + "'!";
	}

	const char* otg::FileInputException::what() const {
		return message.c_str();
	}

}