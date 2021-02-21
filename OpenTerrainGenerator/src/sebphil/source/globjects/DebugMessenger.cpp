#include "globjects/DebugMessenger.h"
#include <iostream>

otg::DebugMessenger::DebugMessenger() {

	glMessageCallback = standardMsgCallback;
	setGlMessageCallback(glMessageCallback);
}

otg::DebugMessenger::DebugMessenger(SEB_GL_MSG_CALLBACK_FUNC_PTR) {

	setGlMessageCallback(this->glMessageCallback);
}

void otg::DebugMessenger::setGlMessageCallback(SEB_GL_MSG_CALLBACK_FUNC_PTR) {

	this->glMessageCallback = glMessageCallback;
	glDebugMessageCallback(this->glMessageCallback, nullptr);
}

void APIENTRY otg::DebugMessenger::standardMsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

	std::string sourceStr = getSourceStr(source);
	std::string typeStr = getSourceStr(type);
	std::string severityStr = getSeverityStr(severity);

	Message msg = {id, message, typeStr, severityStr, sourceStr};

	printMessage(msg);
}

// TODO: too long!
std::string otg::DebugMessenger::getSourceStr(std::uint32_t source) {

	switch (source) {

	case GL_DEBUG_SOURCE_API:
		return "api";

	case GL_DEBUG_SOURCE_APPLICATION:
		return "application";

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "shader-compiler";

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "third party";

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "window system";

	case GL_DEBUG_SOURCE_OTHER:
		return "other";

	default:
		return "not specified";
	}
}

// TODO: too long!
std::string otg::DebugMessenger::getTypeStr(std::uint32_t type) {

	switch (type) {

	case GL_DEBUG_TYPE_ERROR:
		return "error";

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "deprecated behavior";

	case GL_DEBUG_TYPE_MARKER:
		return "marker";

	case GL_DEBUG_TYPE_OTHER:
		return "other";

	case GL_DEBUG_TYPE_PERFORMANCE:
		return "performance";

	case GL_DEBUG_TYPE_POP_GROUP:
		return "pop group";

	case GL_DEBUG_TYPE_PORTABILITY:
		return "portability";

	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "push group";

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "undefined behavior";

	default:
		return "not specified";

	}
}

// TODO: too long!
std::string otg::DebugMessenger::getSeverityStr(std::uint32_t severity) {

	switch (severity) {

	case GL_DEBUG_SEVERITY_LOW:
		return "low";

	case GL_DEBUG_SEVERITY_MEDIUM:
		return "medium";

	case GL_DEBUG_SEVERITY_HIGH:
		return "high";

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "notification";

	default:
		return "not specified\n";

	}
}

void otg::DebugMessenger::printMessage(const Message& msg) {

	std::cout << "OPENGL::DEBUG::A debug-message has been captured: \n";
	std::cout << msg.id << ": " << msg.message << "\n";
	std::cout << "type: " << msg.type << "\n";
	std::cout << "severity: " << msg.severity << "\n";
	std::cout << "source: " << msg.source << "\n";
	std::cout << "OPENGL::DEBUG::end of debug-message \n\n";
}