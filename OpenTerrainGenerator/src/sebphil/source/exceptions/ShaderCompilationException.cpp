#include "exceptions/ShaderCompilationException.h"
#include <vector>
#include <iostream>
#include "glad/glad.h"

namespace otg {

	ShaderCompilationException::ShaderCompilationException(std::uint32_t glHandle, const std::string& filePath) :
		glHandle(glHandle)
	{
		receiveLogMsg();
		constructMsg(filePath);
	}

	void ShaderCompilationException::receiveLogMsg() {

		std::int32_t length = getLogLength();

		std::vector<char> msgLog(length);
		glGetShaderInfoLog(glHandle, length, &length, &msgLog[0]);

		logMsg = std::string(msgLog.begin(), msgLog.end());
	}

	std::int32_t otg::ShaderCompilationException::getLogLength() const {

		std::int32_t length;
		glGetShaderiv(glHandle, GL_INFO_LOG_LENGTH, &length);

		return length;
	}

	void otg::ShaderCompilationException::constructMsg(const std::string& filePath) {

		message = "ERROR::SHADER::COMPILATION::Could not compile shader '" + filePath + "': \n";
		message.append(logMsg);
		message.append("\n");
	}

	const char* otg::ShaderCompilationException::what() const {
		return message.c_str();
	}

}