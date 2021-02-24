#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>

namespace otg {

	class ShaderCompilationException : public std::exception {

	public:
		ShaderCompilationException(std::uint32_t glHandle, const std::string& filePath);

		const char* what() const override;

	private:
		std::uint32_t glHandle;
		std::string message;
		std::string logMsg;

		void receiveLogMsg();
		std::int32_t getLogLength() const;

		void constructMsg(const std::string& filePath);

	};

}
