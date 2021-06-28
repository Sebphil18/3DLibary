#include "exceptions/ImageLoadException.h"

namespace glib {

	ImageLoaderException::ImageLoaderException(const std::string& filePath) {

		message = "ERROR::IMAGE::Could not load '";
		message.append(filePath);
		message.append("'.");
	}

	const char* ImageLoaderException::what() const {
		return message.c_str();
	}

}