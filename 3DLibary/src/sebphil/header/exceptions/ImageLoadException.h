#pragma once
#include <exception>
#include <string>

namespace glib {

	class ImageLoaderException : public std::exception {

	public:
		ImageLoaderException(const std::string& filePath);

		const char* what() const override;

	private:
		std::string message;

	};

}
