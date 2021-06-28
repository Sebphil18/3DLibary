#pragma once
#include <exception>
#include <string>

namespace glib {

	class FileInputException : protected std::exception {

	public:
		FileInputException(const std::string& filePath);

		const char* what() const override;

	private:
		std::string message;

	};

}
