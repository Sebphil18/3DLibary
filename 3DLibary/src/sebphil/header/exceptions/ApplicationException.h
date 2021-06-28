#pragma once
#include <exception>
#include <string>

namespace glib {

	class ApplicationException : public std::exception {

	public:
		ApplicationException(const std::string& description);
		const char* what() const noexcept;

	private:
		std::string message;

	};

}