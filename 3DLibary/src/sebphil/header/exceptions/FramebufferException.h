#pragma once
#include <exception>
#include <string>

namespace otg {

	class FramebufferException : public std::exception {

	public:
		FramebufferException();
		const char* what() const noexcept;

	private:
		std::string message;

	};

}
