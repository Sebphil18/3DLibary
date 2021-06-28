#pragma once
#include <exception>
#include <string>

namespace glib {

	class GlfwExcpetion : public std::exception {

	public:
		GlfwExcpetion(int errCode, const char* description);
		const char* what() const noexcept;

	private:
		std::string message;

	};

}