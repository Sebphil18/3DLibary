#pragma once
#include <exception>
#include <string>

namespace glib {
	
	class UniformBufferException : public std::exception {

	public:
		UniformBufferException(const std::string& blockName);

		const char* what() const override;

	private:
		std::string msg;

	};

}