#include "exceptions/ProgramBuildException.h"
#include <vector>
#include "glad/glad.h"

namespace glib {

	ProgramBuildException::ProgramBuildException(std::uint32_t programHandle):
		programHandle(programHandle)
	{
		constructMessage();
	}

	void ProgramBuildException::constructMessage() {

		std::string log = getLog();

		message = "ERROR:SHADERPROGRAM::BUILD::Program could not be build (there might be more errors associated with this error): \n";
		message.append(log);
		message.append("\n");
	}

	std::string ProgramBuildException::getLog() {

		std::int32_t logLength = getLogLength();

		std::vector<char> logContent(logLength);
		glGetProgramInfoLog(programHandle, logLength, &logLength, &logContent[0]);

		return std::string(logContent.begin(), logContent.end());
	}

	std::int32_t ProgramBuildException::getLogLength() {

		std::int32_t length;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);

		return length;
	}

	const char* ProgramBuildException::what() const {
		return message.c_str();
	}

}