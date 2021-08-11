#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>

namespace glib {

	class ShaderCompilationException : public std::exception {

	public:
		ShaderCompilationException(std::uint32_t glHandle, const std::string& src);

		const char* what() const override;

	private:
		std::uint32_t glHandle;
		std::string message;

		std::string getLog() const;
		std::int32_t getLogLength() const;
		void deleteShader() const;

		void constructMsg(const std::string& src);

	};

}
