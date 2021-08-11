#include "exceptions/ShaderCompilationException.h"
#include <vector>
#include <iostream>
#include "glad/glad.h"

namespace glib {

	ShaderCompilationException::ShaderCompilationException(std::uint32_t glHandle, const std::string& src) :
		glHandle(glHandle)
	{
		constructMsg(src);
		deleteShader();
	}

	void glib::ShaderCompilationException::constructMsg(const std::string& src) {

		std::string log = getLog();

		message = "ERROR::SHADER::COMPILATION::Could not compile shader: \n";
		message.append(log);
		message.append(src);
		message.append("\n");
	}

	std::string ShaderCompilationException::getLog() const {

		std::int32_t logLength = getLogLength();

		std::vector<char> log(logLength);
		glGetShaderInfoLog(glHandle, logLength, &logLength, &log[0]);

		return std::string(log.begin(), log.end());
	}

	std::int32_t ShaderCompilationException::getLogLength() const {

		std::int32_t length;
		glGetShaderiv(glHandle, GL_INFO_LOG_LENGTH, &length);

		return length;
	}

	void ShaderCompilationException::deleteShader() const {
		glDeleteShader(glHandle);
	}

	const char* glib::ShaderCompilationException::what() const {
		return message.c_str();
	}

}